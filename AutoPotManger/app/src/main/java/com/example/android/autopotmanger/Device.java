package com.example.android.autopotmanger;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Scanner;

public class Device extends AppCompatActivity {
    private BluetoothDevice bluetoothDevice;
    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    private static final int REQUEST_ENABLE_BT = 3;
    private BluetoothAdapter bluetoothAdapter;
    private String selectedDeviceMACAddress;
    private BluetoothDeviceManager bluetoothDeviceManager;
    private static final String TAG = "Device View";
    private TextView temperature;
    private TextView air;
    private TextView soil;
    private TextView temperatureValueTextView;
    private TextView airValueTextView;
    private TextView soilValueTextView;
    private Button refreshButton;
    private Button forceButton;
    private int temperatureValue;
    private int moistureValue;
    private int humidityValue;
    private Scanner scanner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        temperature = (TextView)findViewById(R.id.temperature);
        air = (TextView)findViewById(R.id.air_humidity);
        soil = (TextView)findViewById(R.id.soil_humidity);
        temperatureValueTextView=(TextView)findViewById(R.id.temperature_value);
        airValueTextView=(TextView)findViewById(R.id.air_humidity_value);
        soilValueTextView=(TextView)findViewById(R.id.soil_humidity_value);
        refreshButton = (Button)findViewById(R.id.refresh_data);
        forceButton = (Button)findViewById(R.id.force_watering);
        scanner = new Scanner(System.in);
        refreshButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG,"Refresh Clicked");
                String sendDataCommand = "D/";
                sendDataCommand = sendDataCommand + getSpace(16);
                sendMessage(sendDataCommand);
            }
        });
        forceButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG,"Force Clicked");
                String sendDataCommand = "P/";
                sendDataCommand = sendDataCommand + getSpace(16);
                sendMessage(sendDataCommand);
            }
        });
        Intent intent = getIntent();
        if(getIntent().getExtras() != null) {
            selectedDeviceMACAddress = intent.getStringExtra("BLUETOOTH_HARDWARE_ADDRESS");
            connectToDevice();
        }

    }
    private String getSpace(int count){
        String space;
        space = "";
        for(int i=0;i<count;i++)
            space+=" ";
        return space;
    }
    private void connectToDevice() {
        boolean test = BluetoothAdapter.checkBluetoothAddress(selectedDeviceMACAddress);
        BluetoothDevice bluetoothDevice = bluetoothAdapter.getRemoteDevice(selectedDeviceMACAddress);
        if (bluetoothDevice != null) {
            setupChat();
            bluetoothDeviceManager.connect(bluetoothDevice);

        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (bluetoothDeviceManager != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (bluetoothDeviceManager.getState() == BluetoothDeviceManager.STATE_NONE) {
                // Start the Bluetooth chat services
                bluetoothDeviceManager.start();
            }
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (bluetoothDeviceManager != null) {
            bluetoothDeviceManager.stop();
        }
    }

    private StringBuffer InBuffer;
    private StringBuffer OutBuffer;

    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MessageConstants.MESSAGE_STATE_CHANGE:
                    switch (msg.arg1) {
                        case BluetoothDeviceManager.STATE_CONNECTED:
                        case BluetoothDeviceManager.STATE_CONNECTING:
                        case BluetoothDeviceManager.STATE_LISTEN:
                        case BluetoothDeviceManager.STATE_NONE:
                            break;
                    }
                    break;
                case MessageConstants.MESSAGE_WRITE:
                    byte[] writeBuf = msg.toString().getBytes();
                    // construct a string from the buffer
                    String writeMessage = new String(writeBuf);
                    break;
                case MessageConstants.MESSAGE_READ:
                    byte[] readBuf = (byte [])msg.obj;
                    // construct a string from the valid bytes in the buffer
                    String readMessage = null;
                    readMessage = new String(readBuf, 0, msg.arg1);
                    Log.d(TAG,readMessage);
                    scanner= new Scanner(readMessage);
                    scanner.useDelimiter("/");
                    try {
                        if(scanner.hasNext("S")){
                            scanner.next();
                            moistureValue = scanner.nextInt();
                            temperatureValue = scanner.nextInt();
                            humidityValue = scanner.nextInt();
                        }
                    }catch (Exception e){
                            moistureValue = 0;
                            temperatureValue = 0;
                            humidityValue = 0;
                            break;
                    }
                    updateReadings(temperatureValue,humidityValue,moistureValue);
                    break;
                case MessageConstants.MESSAGE_DEVICE_NAME:
                    // save the connected device's name
                    Toast.makeText(Device.this, "Connected to "
                            + selectedDeviceMACAddress, Toast.LENGTH_SHORT).show();
                    break;
                case MessageConstants.MESSAGE_TOAST:
                        Toast.makeText(Device.this, msg.getData().getString(MessageConstants.TOAST),
                                Toast.LENGTH_SHORT).show();
                    break;
            }
        }
    };
    private void setupChat() {
        Log.d(TAG, "setupChat()");
        bluetoothDeviceManager = new BluetoothDeviceManager(this,mHandler);
        OutBuffer = new StringBuffer("");
    }

    /**
     * Sends message to Bluetooth device
     * @param message message to be send to device
     */
    private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (bluetoothDeviceManager.getState() != bluetoothDeviceManager.STATE_CONNECTED) {
            Toast.makeText(this, "Not connected", Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            bluetoothDeviceManager.write(send);

            // Reset out string buffer to zero and clear the edit text field
            OutBuffer.setLength(0);
        }
    }

    /**
     * Update value Text with data provided in arguments
     * @param newTemperature new temperature value
     * @param newHumidity new air humidity value
     * @param newMoisture new soil moisture value
     */
    private void updateReadings(int newTemperature, int newHumidity, int newMoisture){
        airValueTextView.setText(Integer.toString(newHumidity)+"%");
        soilValueTextView.setText(Integer.toString(newMoisture)+"%");
        temperatureValueTextView.setText(Integer.toString(newTemperature)+"\u2103");
    }
}