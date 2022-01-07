## Pepper IoT async API demo
This project is written in standard c. non-standard libraries are provided with the project

[TOCM]

[TOC]


### License
// SmartHome Ventures Confidential
//---------------------------------------
// Copyright 2019-2022 SmartHome Ventures, LLC. All rights reserved.
//
// NOTICE: All information contained herein is and remains the property of
// SmartHome Ventures, LLC. The technical concepts contained herein are
// proprietary to SmartHome Ventures LLC. and may be covered by U.S. and
// foreign patents, or patents in process, and are protected by trade secret
// or copyright law. Dissemination or reproduction of this material is
// strictly forbidden unless prior written permission is obtained from
// SmartHome Ventures, LLC.


### Libraries
- jsmn
- jWrite
- log 

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




