# SEASnet Linux Server Login Helper

### Build
type `make` in terminal to build.
This requires flex and bison. If you want to build yourself, run `make required`. __ELSE__ just directly use the "seas" file included in this repository

To allow `seas` as alias for this helper, type `make mark` to record path in ~/.bash_profile

### Run
type
`./seas`  
if shortcut is set using `make mark`, you can type  
`seas`

### Usage

```
usr *username*
$u *username*
```
set username for common login

```
port *portnum*
$p *portnum*
```
log into specified port

```
dfile port *portnum* *path_1* *path_2*
$f $p *portnum* *path_1* *path_2*
$f $p *portnum* *path_1* => *path_2*
```
download regular file from path_1 in given port to path_2. e.g.
```
dfile port 1 ~/mydir/myfile.txt => . //notice that => is optional
```

```
dfile *path_1* port *portnum* *path_2*
$f *path_1* $p *portnum* *path_2*
```
upload regular file from path_1 to path_2 in the given port

```
ddir ... //(similar rules as above)
$d ...
```
manage directories similar to above

```
exit
```
exit program

### Example
```
$ ./seas
>> SEASnet shortcut v0.1 <<
> usr joebruin
> port 1
> //type password
> //...
> dfile port 9 ~/mydir/myfile.txt .
> //type password
> $d mydir $p 1 ~/mydir_on_lnxsrv
> //type password
> exit
```
