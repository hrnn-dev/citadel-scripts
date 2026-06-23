#!/bin/bash

#=================================================================
# Aprovisionamiento Local: Nodo de pentesting con KVM/QEMU
#================================================================

VM_NAME="pentest-local-node"
RAM_MB=4096
VCPUS=2
DISK_PATH="$HOME/Desarrollo/lab_local/vms/${VM_NAME}.qcow2"

echo ">> Creando disco virtual de 10 GB en $DISK_PATH...."
qemu-img create -f qcow2 $DISK_PATH 10G

echo ">> Desplegando máquina virtual e iniciando instalador de Debian 12..."
virt-install \
    --connect qemu:///system \
    --name $VM_NAME \
    --memory $RAM_MB \
    --vcpus $VCPUS \
    --disk path=$DISK_PATH,format=qcow2 \
    --os-variant debian12 \
    --network network=default \
    --location http://deb.debian.org/debian/dists/bookworm/main/installer-amd64/ \
    --extra-args="console=tty0 console=ttyS0,115200n8" \
    --graphics none
    --serial pty \
    --console pty,target_type=serial \
    --extra-args="console=ttyS0,115200n8"
