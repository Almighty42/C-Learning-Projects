# INI Config File Parser

Parses a .ini config file and either provides the asked value ( --get option ) or sets that key value ( --set option )

## Using the program
### Base form <b>( --get / --set )</b>
```bash
config_parser <config_file>.ini --get <key.value pair>

config_parser <config_file>.ini --set <key.value pair> <new_value>
```
Examples
```bash
config_parser config.ini --get Network.IP
> 192.168.1.1

config_parser config.ini --set Network.Port 8080
> Done setting config.ini
config_parser config.ini --get Network.Port
> 8080
```

### <b>Important note</b> -> --set option overwrites the original file ( all comments, unneccesary spaces are removed )
DO NOT use --set option if you intend to keep the comments of your file

## Building the program
Standard version
```bash
make || make prod
```
Debug version / with debug flags
```bash
make debug
```
Cleaning project executables
```bash
make clean
```
