# SEASnet Linux Server Login Helper

## What's new in V0.9.1
__Fixed Quote & Escape Character "\" Parsing Error!__

## What's new in V0.9
__Support of Automatic login and upload/download (saving your password)!__
__More accurate command names!__
__Nice (actually pretty ugly) Colors!__

## Build
type `make` in terminal to build.
This requires flex and bison. If you want to build yourself, run `make required`. __ELSE__ just directly use the "seas" file included in this repository

To allow `seas` as alias for this helper, type `make mark` to record alias in ~/.bash_profile  
__Warning: the default__ `make mark` __would use .bash_profile as source. (possibly replacing .bashrc)__
__Also, before you use make mark, put this folder at some place appropriate. Later uses of the seas command will be based on the path of your base folder. Avoid moving ./seas around__

To remove the mark, type `make unmark`  
To clean up files used for compilation, type `make clean`

__Notice: the included binary files / Makefile is for OS X only. To build on Linux, you need libmcrypt, libedit, bison and flex, and you are supposed to manually compile yourself (I may consider upload a Ubuntu binary in the future)__

__IMPORTANT: this helper uses mcrypt for encyption of saved password. The key is saved in the source code under Macro__ `PWD_KEY` __. This helper DOES NOT provide safety warranty. If you are kind of worried, modify the macro yourself (into some other numbers, but remember digit chars only, no other type of chars), recompile the program and permanently delete the source file.__

__DO NOT REMOVE seas_expect PROGRAM SINCE AUTO LOGIN IS USING IT!__

## Run
type
`./seas`  
if shortcut is set using `make mark`, you can type  
`seas`

## Usage

### Set username for common login
```
user <username>
usr <username>
`u <username>
```

### Set common server (for future login)
```
server <srvnum>
srv <srvnum>
`s <srvnum>
```

### Set password (for automatic login)
```
setkey
key
`k
password
```

### Delete saved stuff
```
delete key
del key
`d `k

delete user
del usr
`d `u

//no reason to delete saved server number, thus not implemented
```

### Auto login (with saved password)
```
auto login <srvnum>
auto login
`a `l <srvnum>
`a `l
auto <srvnum>
auto
`a <srvnum>
! <srvnum>
!
```

### Login to server
```
login <srvnum>
log <srvnum>
`l <srvnum>
```

### Login to saved server
```
login
log
`l
```

### Download and upload
__WARNING: no server is specified, since it is usually not required (all servers yields the same files)__
```
//download to local
@ <server_path_without_lnxsrv_header> => <local_path>
//or
<local_path> <= @ <server_path_without_lnxsrv_header>

//upload to server
@ <server_path_without_lnxsrv_header> <= <local_path>
//or
<local_path> => @ <server_path_without_lnxsrv_header>
```

### Automatic download and upload (password automatically entered)
```
//download to local
! @ <server_path_without_lnxsrv_header> => <local_path>
//or
! <local_path> <= @ <server_path_without_lnxsrv_header>
//("! can be replaced with `a or "auto")

//upload to server
! @ <server_path_without_lnxsrv_header> <= <local_path>
//or
! <local_path> => @ <server_path_without_lnxsrv_header>
//("!" can be replaced with `a or "auto")
```

### Use BASH without exiting the helper
```
bash
`b
```

### Check settings
```
info
`i
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
[username@port]$ seas
>> SEASnet shortcut v0.9 <<
>> //blah blah blah
> user joebruin
> srv 1
> login
> //type password
> //...
> key
Enter password: ... //type password
* Password saved!
> !
> //automatically logging in
> //...
> @ ~/mydir1/myfile2.txt => localdir
> //type password
> ! @ ~/mydir2 <= localfile
> exit
```
