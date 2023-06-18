# Enabling logging for dekkod {#dekkod_logging}

To enable logging for dekkod, the config parameter `Enabled` in the section `StdStreamLog` of the file `~/.config/dekko2.dekkoproject/dekkod.conf` must be set to `1` like in this snippet:


```
phablet@ubuntu-phablet:~$ cat .config/dekko2.dekkoproject/dekkod.conf 
[FileLog]
Enabled=0
Path=

[LogCategories]
IMAP=1
Messaging=1
POP=1
SMTP=1

[StdStreamLog]
Enabled=1

[Syslog]
Enabled=1
```

After restarting dekkod by issuing `systemctl --user restart dekkod`, the log can
be fetch with `journalctl --user -u dekkod`.

