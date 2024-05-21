Driver sownload: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

pip install esptool

esptool.py -p /dev/cu.SLAB_USBtoUART -b 460800 --before default_reset --after hard_reset --chip esp32 write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x1000 bootloader/bootloader.bin 0x10000 VostokEspIdf.bin 0x8000 partition_table/partition-table.bin