# Bài 1 - Bài 2 -Bài 3
# Bài 1 
DỰ ÁN: BẬT TẮT LED BẰNG NGẮT NGOÀI VÀ LED NHẤP NHÁY BẰNG TIMER GIẢ LẬP
Vi điều khiển: STM32F103C8T6 (Blue Pill)
Thư viện: Standard Peripheral Library (SPL)
🎯 Mục tiêu
Chương trình này minh họa:
- Task 1: LED1 (PA0) nhấp nháy liên tục (blink) bằng delay thô.
- Task 2: LED2 (PB12) được bật/tắt bằng nút nhấn PB13 thông qua ngắt ngoài (EXTI).
- Khi nhấn nút → LED sáng.
- Khi thả nút → LED tắt.
⚙️ Cấu hình phần cứng:
Thành phần	Chân STM32	Chức năng	Ghi chú
- LED1	: PA0 ->	Blink liên tục -> gắn điện trở 220Ω nối LED xuống GND
- LED2	:PB12	-> Điều khiển bằng nút nhấn	Dạng Output
- Nút nhấn	PB13	Tạo ngắt ngoài	Nối GND khi nhấn
- GND	-	Dây chung mạch
🧩 Cấu trúc chương trình
1️⃣ LED_Init()
- Bật clock cho GPIOA và GPIOB.
Cấu hình:
- PA0: Output push-pull (LED nhấp nháy).
- PB12: Output push-pull (LED điều khiển bằng nút).
2️⃣ Button_Init()
- Bật clock cho GPIOB và AFIO.
- Cấu hình PB13 làm Input Pull-up.
- Kết nối PB13 tới EXTI Line13.
- Cấu hình ngắt ngoài EXTI15_10_IRQn kích hoạt ở cả hai cạnh (rising/falling).
- Cho phép ngắt trong NVIC.
3️⃣ EXTI15_10_IRQHandler()
- Hàm xử lý ngắt ngoài:
- Kiểm tra xem ngắt đến từ PB13 hay không.
- Nếu PB13 = 0 (nhấn nút): Bật LED PB12.
- Nếu PB13 = 1 (thả nút): Tắt LED PB12.
- Xóa cờ ngắt để chuẩn bị cho lần sau.
4️⃣ Task_Blink()
- Đảo trạng thái LED PA0.
- Tạo delay thô (bằng vòng lặp) để LED nhấp nháy chậm.
5️⃣ main()
Gọi SystemInit() để khởi tạo hệ thống clock.
Khởi tạo LED và nút nhấn.
Trong vòng lặp while(1), LED PA0 nhấp nháy liên tục, ngắt ngoài PB13 hoạt động song song.
⚡ Hoạt động chương trình
Khi cấp nguồn hoặc reset:
→ LED PA0 bắt đầu nhấp nháy liên tục.
Khi nhấn nút PB13:
→ LED PB12 sáng.
Khi thả nút PB13:
→ LED PB12 tắt.
Ngắt ngoài hoạt động độc lập với vòng lặp chính, không làm ảnh hưởng đến quá trình nhấp nháy LED1.
# BÀI 2
🧠 ĐỀ BÀI : GỬI DỮ LIỆU UART TỪ HAI TASK CHẠY SONG SONG TRÊN FREERTOS
Vi điều khiển: STM32F103C8T6 (Blue Pill)
Thư viện: STM32 Standard Peripheral Library (SPL) + FreeRTOS
🎯 Mục tiêu
Chương trình minh họa cách tạo và chạy nhiều task song song trong FreeRTOS trên STM32.
Mỗi task sẽ gửi chuỗi thông báo riêng của nó qua cổng UART1 để hiển thị trên Serial Monitor.
- Task 1: Gửi chuỗi "Task 1 sending data..."
- Task 2: Gửi chuỗi "Task 2 sending data..."
- Hai task cùng hoạt động song song, mỗi 200 ms gửi một lần.
- Kết nối USB–UART:
- STM32 (PA9)  --->  USB–UART (RX)
- STM32 (PA10) --->  USB–UART (TX)
-STM32 (GND)  --->  USB–UART (GND)
🧩 Cấu trúc chương trình
 1️ USART1_Init()
- Bật clock cho GPIOA và USART1.
- Cấu hình chân:
- PA9 (TX): Alternate Function Push–Pull.
- PA10 (RX): Input Floating.
- Thiết lập UART1 với:
- Baud rate: 9600
8 bit data, 1 stop bit, no parity, no flow control.
- Kích hoạt UART bằng USART_Cmd(USART1, ENABLE).
- 2️ sendString(char *str)
- Hàm gửi chuỗi qua UART1.
- Gửi từng ký tự.
- Chờ đến khi thanh ghi truyền (TXE) sẵn sàng.
- Dừng khi gặp ký tự kết thúc chuỗi (\0).
3️ Task1()
- Chạy trong vòng lặp vô hạn.
- Gửi chuỗi "Task 1 sending data...\r\n".
- Sau đó delay 200 ms bằng vTaskDelay().
4️ Task2()
- Hoạt động tương tự Task1.
- Gửi chuỗi "Task 2 sending data...\r\n".
- Delay 200 ms giữa mỗi lần gửi.
5️ main()
- Gọi SystemInit() để khởi tạo hệ thống clock.
- Khởi tạo UART1.
- Tạo hai task:
- xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
- xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);
- kết quả :
2 task bị xung đột , hiển thị lên uart thì bị gửi những kí tự lạ không giống với 2 chuỗi đã tạo.
# BÀI 3
# 🧠 ĐỀ BÀI : GỬI DỮ LIỆU UART TỪ NHIỀU TASK SỬ DỤNG MUTEX TRONG FREERTOS

**Vi điều khiển:** STM32F103C8T6 (Blue Pill)  
**Thư viện:** STM32 Standard Peripheral Library (SPL) + FreeRTOS  

---

## 🎯 MỤC TIÊU

Chương trình minh họa cách sử dụng **Mutex (Mutual Exclusion Semaphore)** trong FreeRTOS  
để **bảo vệ tài nguyên dùng chung** – cụ thể ở đây là **UART1**.

Nếu không có Mutex, khi nhiều task cùng gửi dữ liệu qua UART, dữ liệu sẽ **chồng chéo, nhiễu loạn**.  
Nhờ Mutex, chỉ **một task được phép truy cập UART** tại một thời điểm.

---

## ⚙️ CẤU HÌNH PHẦN CỨNG

| Chân STM32 | Kết nối | Mô tả |
|-------------|----------|--------|
| PA9  | TX → RX của USB–UART | Gửi dữ liệu từ STM32 sang máy tính |
| PA10 | RX → TX của USB–UART | Nhận dữ liệu (không dùng trong bài này) |
| GND  | GND chung | Kết nối mass |

**Sơ đồ nối cơ bản:**

## 🧩 CẤU TRÚC CHƯƠNG TRÌNH

### 1️⃣ `USART1_Init()`
- Bật clock cho **GPIOA** và **USART1**.  
- Cấu hình chân:
  - **PA9 (TX):** Alternate Function Push-Pull.  
  - **PA10 (RX):** Input Floating.  
- Cấu hình UART1:
  - Baud rate: `9600`
  - 8 data bits, 1 stop bit, no parity  
  - Không dùng flow control  
- Kích hoạt UART bằng `USART_Cmd(USART1, ENABLE)`.
---
### 2️⃣ `sendString(char *str)`
Hàm gửi chuỗi qua UART1:
- Gửi từng ký tự, chờ thanh ghi TXE trống.  
- Dừng khi gặp ký tự `\0`.

---

### 3️⃣ `main()`
- Khởi tạo hệ thống và UART.  
- **Tạo một Mutex:**  
  ```c
  xMutex = xSemaphoreCreateMutex();
Nếu Mutex tạo thành công, tạo 2 task:
c
Sao chép mã
xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);
kết quả :
đã thành công hiển thị 2 task lên màn hình.
