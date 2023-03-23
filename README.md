# ph_environmental_cabinet_controller_io_fw

## Thông số kỹ thuật

### Tính năng 

- Nguồn cấp 24V
- 1 giao tiếp 485 Salver (A1, B1): kết nối với màn hình điều khiển (ID = 1)
- 1 giao tiếp 485 Master (A2, B2): Kết nối với 11 thiết bị(10 cảm biến SHT31 ID = (1 - 10), 1 máy lạnh Daikin (ID = 11))
- 12 output relay nối chung 24V(khóa cửa, đèn chiếu sáng, đèn cảnh báo (đỏ, vàng, xanh), tủ tách ẩm, điện trở đốt nóng, mắt phun sương, )
- 1 input cảm biến cửa.


### Giao thức ModbusRTU kết nối với board MCU tủ môi trường 

#### Bảng địa chỉ RS485 modbus RTU với board MCU tủ môi trường

ID: Function code: 04 (Read only)
| Địa chỉ | Kiểu              | Tên       							 | Mô tả
| ------- | ----------------  | -------------------------------------|---------------------
| 0       | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 1
| 1       | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 1
| 2       | Int (Read only)   | status1                              | cảm biến nhiệt độ, độ ẩm 1 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 5       | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 2
| 6       | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 2
| 7       | Int (Read only)   | status2                              | cảm biến nhiệt độ, độ ẩm 2 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 10      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 3
| 11      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 3
| 12      | Int (Read only)   | status3                              | cảm biến nhiệt độ, độ ẩm 3 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 15      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 4
| 16      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 4
| 17      | Int (Read only)   | status4                              | cảm biến nhiệt độ, độ ẩm 4 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 20      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 5
| 21      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 5
| 22      | Int (Read only)   | status5                              | cảm biến nhiệt độ, độ ẩm 5 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 25      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 6
| 26      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 6
| 27      | Int (Read only)   | status6                              | cảm biến nhiệt độ, độ ẩm 6 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 30      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 7
| 31      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 7
| 32      | Int (Read only)   | status7                              | cảm biến nhiệt độ, độ ẩm 7 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 33      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 8
| 34      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 8
| 35      | Int (Read only)   | status8                              | cảm biến nhiệt độ, độ ẩm 8 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 40      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 9
| 41      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 9
| 42      | Int (Read only)   | status9                              | cảm biến nhiệt độ, độ ẩm 9 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 45      | Int (Read only)   | giá trị nhiệt độ                     | cảm biến nhiệt độ, độ ẩm 10
| 46      | Int (Read only)   | giá trị độ ẩm                        | cảm biến nhiệt độ, độ ẩm 10
| 47      | Int (Read only)   | status10                             | cảm biến nhiệt độ, độ ẩm 10 (0: Good ; 1: check connection; 2; over range)
|         |                   |                                      |
| 50      | Int (Read only)   | Nhiệt độ trung bình trong phòng      | nhiệt độ trung bình của 8 cảm biến trong phòng
| 51      | Int (Read only)   | Độ ẩm trung bình trong phòng         | độ ẩm trung bình của 8 cảm biến trong phòng

| 55      | Int (Read only)   | SENSOR DOOR                         | 0: cửa mở, 1: cửa đóng


ID: function code: 03, 06 (Read/Wite)

| Địa chỉ | Kiểu             | Tên       							| Mô tả
| ------- | ---------------- | -------------------------------------|---------------------
| 0       | Int (Read/Wite)  | STATUS COOLER                        | 0: OFF, 1: On, 2: Warning
|         |                  |                                      |
| 5       | Int (Read/Wite)  | STATUS HUMIDIFY                      | 0: OFF, 1: On, 2: Warning
| 6       | Int (Read/Wite)  | STATUS MOISER                        | 0: OFF, 1: On, 2: Warning
| 7       | Int (Read/Wite)  | STATUS THERMISTOR 1                  | 0: OFF, 1: On
| 8       | Int (Read/Wite)  | STATUS THERMISTOR 2                  | 0: OFF, 1: On
| 9       | Int (Read/Wite)  | STATUS THERMISTOR 3                  | 0: OFF, 1: On
| 10      | Int (Read/Wite)  | STATUS LIGHT                         | 0: OFF, 1: On
| 11      | Int (Read/Wite)  | STATUS LIGHT GREEN                   | 0: OFF, 1: On
| 12      | Int (Read/Wite)  | STATUS LIGHT RED                     | 0: OFF, 1: On
| 13      | Int (Read/Wite)  | STATUS LIGHT YELLOW                  | 0: OFF, 1: On
| 14      | Int (Read/Wite)  | CLOCK THE DOOR                       | 0: OFF, 1: On
| 15      | Int (Read/Wite)  | VENTILATOR VALVE                     | 0: OFF, 1: On
| 16      | Int (Read/Wite)  | COOLER AIR VALVE                     | 0: OFF, 1: On
| 17      | Int (Read/Wite)  | HEATER AIR VALVE                     | 0: OFF, 1: On
|         |                  |                                      |
| 20      | Int (Read/Wite)  | ROOM TEMPERATURE SETTING             | setup: 20 - 80℃
| 21      | Int (Read/Wite)  | ROOM HUMIDITY SETTING                | setup: 30 - 100℃
|         |                  |                                      |
| 25      | Int (R/W)        | STATUS START/STOP            		| 0: START, 1: STOP
|         |                  |                                      |
| 30      | Int (R/W)        | Giá trị calib temp Y1       			| Cảm biến 1
| 31      | Int (R/W)        | Giá trị calib temp Y2         	    | Cảm biến 1
| 32      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 1
| 33      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 1
| 34      | Int (R/W)        | Calculator a, b  cảm biến 1  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 35      | Int (R/W)        | Giá trị calib Y1         			| Cảm biến 2
| 36      | Int (R/W)        | Giá trị calib Y2         			| Cảm biến 2
| 37      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 2
| 38      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 2
| 39      | Int (R/W)        | Calculator a, b  cảm biến 2  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 40      | Int (R/W)        | Giá trị calib temp Y1       			| Cảm biến 3
| 41      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 3
| 42      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 3
| 43      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 3
| 44      | Int (R/W)        | Calculator a, b  cảm biến 3 			| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 45      | Int (R/W)        | Giá trị calib temp Y1       			| Cảm biến 4
| 46      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 4
| 47      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 4
| 48      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 4
| 49      | Int (R/W)        | Calculator a, b  cảm biến 4  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 50      | Int (R/W)        | Giá trị calib temp Y1       			| Cảm biến 5
| 51      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 5
| 52      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 5
| 53      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 5
| 54      | Int (R/W)        | Calculator a, b  cảm biến 5  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 55      | Int (R/W)        | Giá trị calib temp Y1         	    | Cảm biến 6
| 56      | Int (R/W)        | Giá trị calib temp Y2         		| Cảm biến 6
| 57      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 6
| 58      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 6
| 59      | Int (R/W)        | Calculator a, b  cảm biến 6  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 60      | Int (R/W)        | Giá trị calib temp Y1       			| Cảm biến 7
| 61      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 7
| 62      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 7
| 63      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 7
| 64      | Int (R/W)        | Calculator a, b  cảm biến 7  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 65      | Int (R/W)        | Giá trị calib temp Y1         		| Cảm biến 8
| 66      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 8
| 67      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 8
| 68      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 8
| 69      | Int (R/W)        | Calculator a, b  cảm biến 8  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 70      | Int (R/W)        | Giá trị calib temp Y1        		| Cảm biến 9
| 71      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 9
| 72      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 9
| 73      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 9
| 74      | Int (R/W)        | Calculator a, b  cảm biến 9  		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am
| 75      | Int (R/W)        | Giá trị calib temp Y1        		| Cảm biến 10
| 76      | Int (R/W)        | Giá trị calib temp Y2       			| Cảm biến 10
| 77      | Int (R/W)        | Giá trị calib humi Y1                | Cảm biến 10
| 78      | Int (R/W)        | Giá trị calib humi Y2                | Cảm biến 10
| 79      | Int (R/W)        | Calculator a, b  cảm biến 10 		| 0: không làm gì; 1: hiệu chuẩn nhiệt độ; 2: hiệu chuẩn độ ẩm; 3: reset nhiet do; 4: reset do am

#### Note: 
- Cảm biến nhiệt độ, độ ẩm số 5,6 nằm trên nóc tủ
- Cảm biến nhiệt độ, độ ẩm số 1,2,3,4,7,8,9,10 trải đều các góc trong tủ
- Giá trị đọc được từ thanh ghi nhiệt độ là 304 => nhiệt độ thực tế = 30.4
- Giá trị độ ẩm tương tự như nhiệt độ

--------------------------------------------------------------

Ví dụ: calib linear regression
* pointx(x,y);

- point1(x1,y1);
- point2(x2,y2);

=> calculator a,b
=> phương trình calib y' = a + bx'

- Gửi giá trị Y1 -> Lấy X1
- Gửi giá trị Y2 -> Lấy X2
- Chờ thanh ghi Xác nhận (của từng cảm biến), check X1 và X2 (cả 2 đều phải khác 0)

1 cảm biến sẽ có:
- 2 giá trị Y của nhiệt độ
- 2 giá trị Y của độ ẩm
- 1 thanh ghi xác nhận: 0 - ko làm gì cả, 1 - hiệu chuẩn nhiệt độ, 2 - hiệu chuẩn độ ẩm (hiệu chuẩn xong reset giá trị các thanh ghi)

status = 0 : không có gì
status = 1 : lỗi không nhận được tín hiệu cảm biến
status = 2 : lỗi over range
--------------------------------------------------------------

#### Ví dụ:

##### SHT31 Sensor temperature humidity

Note: đổi địa chỉ cảm biến :
5 -> 9
6 -> 10


Master gởi yêu cầu nhận giá trị nhiệt, độ ẩm
` ` ` 01 03 00 00 00 02 C4 0B ` ` `
**01**: ID SHT31
**03**: Function code (Read holding registers)
**00**: Địa chỉ bắt đầu bit cao
**00**: Địa chỉ bắt đầu bit thấp
**00**: Số lượng dữ liệu bit cao
**02**: số lượng dữ liệu bit thấp
**C4**: Mã check CRC bit cao
**B0**: Mã check CRC bit thấp

Master nhận được response
` ` ` 01 03 04 01 25 01 AE 6A 28 ` ` `
**01**: ID SHT31
**03**: Function code (Read holding registers)
**04**: Data lengh
**01 25**: => 293/10 = 29.3 độ C
**01 AE**: => 430/10 = 43.0 %
**6A**: bit CRC
**28**: bit CRC

--------------------------------------------------------------

##### Calib sensor 
Temperature Correction: 106 range( -10 ~ 10 )
` ` ` 01 06 00 6A FF FF A8 66 ` ` `
**01**: ID SHT31
**06**: Function code (Write holding registers)
**04**: Data lengh
**00 6A**: Addrest register 106
**FF FF**: Data register -1
**A8**: bit CRC
**66**: bit CRC

` ` ` 01 06 00 6A FF FE 69 A6 ` ` `
**01**: ID SHT31
**06**: Function code (Write holding registers)
**04**: Data lengh
**00 6A**: Addrest register 106
**FF FE**: Data register -2
**69**: bit CRC
**A6**: bit CRC

--------------------------------------------------------------
### Note relay
 RL1(1); // quat hut tuan hoan dk tat sau khoang lanh va khoang nong
 RL2(1); // den chieu sang
 RL3(1); // den bao do
 RL4(1); // den bao xanh
 RL11(1); // den bao vang
 RL6(1); // chet con nay roi
 RL7(1); // khoa cua
 RL8(1); // dien tro nhiet 1
 RL9(1); // dien tro nhiet 2
 RL10(1); // dien tro nhiet 3
 RL12(1); // Control may hut am
 RL13(1); // mat phun suong
 RL14(0); // van khi lanh
 RL15(1); // van khi nong


