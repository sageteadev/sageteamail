# Core Dumps {#core_dumps}

If Dekko is crashing for you and we're not able to reproduce it on our devices/with our mail accounts, you can help fixing the crash by sending us a core dump.

Attention: the core dump will contain sensitive data - it may even contain your mail password. So don't send us your core dump, if you don't trust us. ;-) And better change your mail password afterwards or even better try to reproduce the crash with a test account.

If you still want to send us a core dump:

1. Get a debug build of dekko
2. Do whatever is crashing dekko for you
3. You'll find a core dump on the device: `.local/share/dekko2.dekkoproject/core`
4. Compress the core dump i.e. with gzip `gzip .local/share/dekko2.dekkoproject/core`, this should reduce its size to about 15 MB - small enough to send it over (ofc you can use your favourite zip tool instead)
5. Finally send us the compressed core dump

