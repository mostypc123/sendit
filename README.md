# `sendit`

`sendit` is a serverless chat app written in C++. It is basically a fancy file writer and reader.
It is planned to expand it to a GUI app and give it support for connecting with SSH, in both the GUI and the CLI.

## Usage

Build from source:

```bash
make -j$(nproc)
```

To create a new "instance", just send a message in a new directory. If you are root, sendit automatically uses `/run/sendit`.

> TODO: support custom directories, should be easy to just edit the StateDir class

The admin can send admin messages (the admin functionality will expand). You need to set an admin password:

```bash
sendit -m admin-setpwd # interactive
sendit -m admin-setpwd:password # non-interactive (obv)
```
