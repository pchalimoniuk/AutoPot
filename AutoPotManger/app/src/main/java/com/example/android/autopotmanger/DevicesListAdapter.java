package com.example.android.autopotmanger;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.RecyclerView;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.Set;

/**
 * {@link DevicesListAdapter} exposes a list of bluetooth devices to a
 * {@link androidx.recyclerview.widget.RecyclerView;}
 */
public class DevicesListAdapter extends RecyclerView.Adapter<DevicesListAdapter.DeviceListAdapterViewHolder> {
    private ArrayList<String> deviceName;
    private ArrayList<String> deviceMAC;
    private final DeviceListAdapterOnClickHandler mClickHandler;
    int selectedPosition = 0;
    private View parentView;
    public DevicesListAdapter(Set<BluetoothDevice> bluetoothDevices, DeviceListAdapterOnClickHandler clickHandler) {
        mClickHandler = clickHandler;
        setDevicesData(bluetoothDevices);
    }
    public DevicesListAdapter(DeviceListAdapterOnClickHandler clickHandler){
        mClickHandler= clickHandler;
        deviceName = new ArrayList<String>();
    }

    @NonNull
    @Override
    public DeviceListAdapterViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        parentView = parent;
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.bluetooth_item, parent, false);
        return new DeviceListAdapterViewHolder(v);
    }

    @SuppressLint("ResourceAsColor")
    @Override
    public void onBindViewHolder(@NonNull DeviceListAdapterViewHolder holder, int position) {
        holder.getTextView().setText(deviceName.get(position));
        if(selectedPosition == position){
            holder.getTextView().setBackgroundColor(ContextCompat.getColor(parentView.getContext(),R.color.colorSelectedItem));
        }else{
            holder.getTextView().setBackgroundColor(ContextCompat.getColor(parentView.getContext(),R.color.colorNonSelectedItem));
        }
    }

    @Override
    public int getItemCount() {
        return deviceName.size();
    }
    public interface DeviceListAdapterOnClickHandler{
        void onClick(String deviceMACAddress);
    }
    public class DeviceListAdapterViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private final TextView mTextView;

        public DeviceListAdapterViewHolder(View v) {
            super(v);
            // Define click listener for the ViewHolder's View.
            v.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.d("Bluetooth device: ", "Element " + getAdapterPosition() + " clicked.");
                    int adapterPosition = getAdapterPosition();
                    String selectedDeviceMAC = deviceMAC.get(adapterPosition);
                    selectedPosition = adapterPosition;
                    mClickHandler.onClick(selectedDeviceMAC);
                    notifyDataSetChanged();
                }
            });
            mTextView = (TextView) v.findViewById(R.id.bluetooth_device_item);
        }
        public TextView getTextView(){
            return mTextView;
        }

        @Override
        public void onClick(View v) {

        }
    }
    public void setDevicesData(Set <BluetoothDevice> bluetoothDevices){
        if(bluetoothDevices != null) {
            deviceName = new ArrayList<String>(bluetoothDevices.size());
            deviceMAC = new ArrayList<String>(bluetoothDevices.size());
            if (bluetoothDevices != null && !bluetoothDevices.isEmpty()) {
                for (BluetoothDevice bD : bluetoothDevices) {
                        deviceName.add(bD.getName());
                        deviceMAC.add(bD.getAddress());
                }
            }
        }else{
            deviceName = new ArrayList<String>();
            deviceMAC = new ArrayList<String>();
        }
        notifyDataSetChanged();
    }
    public void addDevice(BluetoothDevice bluetoothDevice){
        if(bluetoothDevice != null){
            deviceName.add(bluetoothDevice.getName());
            deviceMAC.add(bluetoothDevice.getAddress());
        }
        notifyDataSetChanged();
    }
    public String getDeviceAddress(int position){
        if(deviceMAC.size() >= position){
            return deviceMAC.get(position-1);
        }
        return null;
    }
}
