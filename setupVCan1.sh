#!/bin/bash

# Load vcan kernel module
if ! sudo modprobe vcan; then
  echo "Error: Failed to load vcan kernel module"
  exit 1
fi

# Add vcan1 interface
if ! sudo ip link add dev vcan1 type vcan 2>/dev/null; then
  # Check if interface already exists
  if ip link show vcan1 &>/dev/null; then
    echo "Warning: vcan1 interface already exists"
  else
    echo "Error: Failed to add vcan1 interface"
    exit 1
  fi
fi

# Bring up vcan1 interface
if ! sudo ip link set up vcan1; then
  echo "Error: Failed to bring up vcan1 interface"
  exit 1
fi

echo "Success: vcan1 is ready to use!"