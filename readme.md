## Pepper IoT async API demo
This project is written in standard c. non-standard libraries are provided with the project

[TOCM]

[TOC]

### Libraries
- jsmn
- jWrite
- log 
### Aditionally 
Follow instructions on PAHO Mqtt page for your platform.
https://www.eclipse.org/paho/index.php?page=clients/c/index.php

### to build 
`$ make
`
### to run 
`$ main
`

### Required
- config file required 
		device.cfg contains key,value pair in plain text, no spaces
			look in the repo for example key,value pairs
- pem file required 
		ca.pem provided by pepper
both files reside in the directory level where the program resides. ie: ./


### AsyncAPI Documentation 
For the asyncapi [documentation](https://www.asyncapi.com/tools/generator "documentation") -
```bash
npm install -g @asyncapi/generator 
cd doc
ag ./registerUserServcie.yaml @asyncapi/html-template -o PepperDeviceApi --force-write
```




