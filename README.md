# PipeOverNet

## Intro
command pipe over network

## Usage
```
pon send|recv host[:port]
```

## Sample
if you want to backup/restore btrfs snapshot to other PC over network

* PC1:192.168.1.50
* PC2:192.168.1.100
```
root@PC2:~# pon recv 0.0.0.0 | btrfs receive $BTRFS_PATH
```
```
root@PC1:~# btrfs send $BTRFS_PATH | pon send 192.168.1.100
```
