# effective-packet-filter

## Resources

[Wiki Nftables - Netfilter hooks](https://wiki.nftables.org/wiki-nftables/index.php/Netfilter_hooks)

## Usefull commands
``` bash
# DNS query
dig yahoo.com
```

```bash
# Capture outgoing DNS requests
sudo tcpdump -i wlan0  -n -s 0 -vvv 'dst port 53'
```