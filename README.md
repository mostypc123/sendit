# `sendit`

`sendit` is a serverless chat app written in C++. It is basically a fancy file writer and reader.
It is planned to expand it to a GUI app and give it support for connecting with SSH, in both the GUI and the CLI.

## Usage

Build from source:

```bash
make -j$(nproc)
```

To create a new "instance", just send a message in a new directory. If you are root, sendit automatically uses `/run/sendit`.
Otherwise, it uses `./.sendit`.

> TODO: support custom directories, should be easy to just edit the StateDir class

## Admin

The admin can send admin messages (the admin functionality will expand). You need to set an admin password:

```bash
sendit -m admin-setpwd # interactive
sendit -m admin-setpwd:password # non-interactive (obv)
```
It is better to use interactive mode as it is safer.

The password is hashed with SHA256 and stored in the statedir in the file "admin-pwd".

## Sending a message using CLI

To send messages as a user (including admin - for admin messages it asks for the password):

```bash
sendit -m message -n MyName
```

If no name is set, `anonymous` is used.

## Reading messages

To read all messages, just run sendit:

```bash
sendit
```

While the reader is running, it assigns a color to every user that does not have one.

To use a proper client, use sendit with the `-i` flag.

## motd

To set a welcome text, send this as admin:
```
admin-setmotd:welcome text...
```

Then unset:

```
admin-setmotd:unset
```

---

The .hh files include implementations because i am NOT going to use the ugly classname::funcname() {code} syntax. Thank you.
