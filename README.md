# highest-cpu-usage
Custom waybar module to show the process with the highest CPU usage

## usage
### build
Run:
```bash
# make 
```
and install the binary with: 
```bash
# make install
```

### config
Within your waybar config, define a custom module, for example:
```json
    "custom/hcu": {
		"format": "{}",
		"interval": 1,
		"max-length": 20,
		"exec": "~/.config/waybar/hcu"
	},
```
Then add *"custom/hcu"* to modules-left, modules-right, or modules-center.
