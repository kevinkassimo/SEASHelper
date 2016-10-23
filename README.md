# SEASnet Linux Server Login Helper

## What's new in V0.2
__Now cursor could be moved normally!__  
__Also, history has been implemented. You can press UP arrow key to browse history.__

## Build
type `make` in terminal to build.
This requires flex and bison. If you want to build yourself, run `make required`. __ELSE__ just directly use the "seas" file included in this repository

To allow `seas` as alias for this helper, type `make mark` to record path in ~/.bash_profile

## Run
type
`./seas`  
if shortcut is set using `make mark`, you can type  
`seas`

## Usage

### Set username for common login
```
user <username>
`u <username>
```

### Set common port (for future login)
```
port <portnum>
`p <portnum>
```

### Login to port
```
login <portnum>
log <portnum>
`l <portnum>
```

### Login to saved port
```
login
log
`l
```

### SIMPLIFIED download and upload
__WARNING: no port is specified, since it is usually not required (all ports yields the same files)__
```
//download to local
@ <server_path_without_lnxsrv_header> => <local_path>
//or
<local_path> <= @ <server_path_without_lnxsrv_header>
//@ is replaceable with "server" or "srv"

//upload to server
@ <server_path> <= <local_path>
//or
<local_path> => @ <server_path_without_lnxsrv_header>
//@ is equivalent to "server" or "srv"

```

### Original download and upload
```
fcp port <portnum> <path_1> <path_2>
`f `p <portnum> <path_1> <path_2>
```
download regular file from path_1 in given port to path_2. e.g.
```
fcp port 1 ~/mydir/myfile.txt .
```

```
fcp <path_1> port <portnum> <path_2>
`f <path_1> $p <portnum> <path_2>
```
upload regular file from path_1 to path_2 in the given port

```
dcp ... //(similar rules as above)
`d ...
```
manage directories similar to above

### Check settings
```
stat
`s
```

### Getting help
```
help
`h
```

### Visit Github repository
```
repo
`r
```

### Exit program
```
exit
```

## Example
```
$ ./seas
>> SEASnet shortcut v0.1 <<
> user joebruin
> port 1
> //type password
> //...
> @ ~/mydir1/myfile2.txt => localdir
> //type password
> fcp port 9 ~/mydir/myfile.txt .
> //type password
> `d dcp `p 1 ~/mydir_on_lnxsrv
> //type password
> exit
```
