package com.example.android.autopotmanger;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ProgressBar;

import androidx.annotation.Nullable;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.util.Set;

public class BluetoothActivity extends Activity implements DevicesListAdapter.DeviceListAdapterOnClickHandler {
    private final static String TAG = "BLUETOOTH ACTIVITY";
    DevicesListAdapter devicesListAdapter;
    private final BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG, "Receiver made");
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                Log.d(TAG, "Device founded");
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device != null) {
                    if (device.getName() != null && !device.getName().isEmpty()) {
                        devicesListAdapter.addDevice(device);
                        if(devicesListAdapter.getItemCount() == 1 && devicesListAdapter.getDeviceAddress(1) !=null){
                            selectedDeviceMACAddress = devicesListAdapter.getDeviceAddress(1);
                        }
                    }
                }
            }
        }
    };
    String selectedDeviceMACAddress;
    int REQUEST_ENABLE_BT = 1;
    BluetoothAdapter bluetoothAdapter;
    private RecyclerView mRecyclerView;
    private ProgressBar mLoadingIndicator;
    private Button mScanButton;
    private Button mSelectButton;
    private FrameLayout mButtonsLayout;

    public int prepareBluetooth() {
        if (bluetoothAdapter == null) {  //device does not support Bluetooth
            Log.i(TAG, "Device does not support Bluetooth");
            return -1;
        }
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            int activityResponse = 100;
            onActivityResult(REQUEST_ENABLE_BT, activityResponse, null);
            if (activityResponse == RESULT_CANCELED) {
                Log.i(this.getClass().getSimpleName(), "Failed to turn on Bluetooth");
                return -1;
            }
        }
        return 1;
    }

    Set<BluetoothDevice> getPairedBluetoothDevices() {
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                String deviceName = device.getName();
                String deviceMACAddress = device.getAddress(); //MAC address;
                //TODO(1) Make text list to list paired devices
            }
        }
        return pairedDevices; //TODO(2) Figure out is retuning set necessary
    }

    public void scanDevices() {
        Log.d(TAG, "Scan devices method called");
        bluetoothAdapter.startDiscovery();
        if (bluetoothAdapter.isDiscovering()) {
            Log.d(TAG, "Bluetooth is discovering devices");
            bluetoothAdapter.cancelDiscovery();
        }
    }

    public void stopScanning() {
        bluetoothAdapter.cancelDiscovery();
    }


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);
        mRecyclerView = (RecyclerView) findViewById(R.id.bluetooth_device_list);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        mRecyclerView.setHasFixedSize(true);
        mRecyclerView.setLayoutManager(linearLayoutManager);
        devicesListAdapter = new DevicesListAdapter(this);
        mRecyclerView.setAdapter(devicesListAdapter);
        mLoadingIndicator = (ProgressBar) findViewById(R.id.pb_loading_indicator);
        mButtonsLayout = (FrameLayout) findViewById(R.id.button_frame_layout);
        mSelectButton = (Button) findViewById(R.id.button_select);
        mScanButton = (Button) findViewById(R.id.button_scan_devices);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
//        if (prepareBluetooth() == 1) {
//            devicesListAdapter.setDevicesData(getPairedBluetoothDevices());
//        }
        mScanButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                devicesListAdapter.setDevicesData(null);
                scanDevices();
            }
        });
        mSelectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG, ": Select button clicked");
                stopScanning();
                deviceSelected();
            }
        });
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(receiver, filter);


    }


    @Override
    public void onClick(String deviceMACAddress) {
        selectedDeviceMACAddress = deviceMACAddress;
        mSelectButton.setEnabled(true);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(receiver);
    }
    private void deviceSelected(){
        Intent intent = new Intent(this, Device.class);
        intent.putExtra("BLUETOOTH_HARDWARE_ADDRESS",selectedDeviceMACAddress);
        startActivity(intent);
    }

}
