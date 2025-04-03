import serial
import csv
import os

TEST_NO = int(input("Enter the Test No : "))

ser = serial.Serial("COM5", 9600, timeout=1)

csv_filename = "data.csv"

file_exists = os.path.exists(csv_filename)

try:
    with open(csv_filename, "a", newline="") as file:
        writer = csv.writer(file)

        if not file_exists:
            writer.writerow(["TestNo","MainTemp", "LampTemp", "Intensity", "DefectFlag"])

        while True:
            line = ser.readline().decode("utf-8").strip()
            if line and not line.startswith("MainTemp"):  
                if "," in line:
                    data=line.split(",")
                    data.insert(0,TEST_NO)
                    writer.writerow(data)
                print(line)

except KeyboardInterrupt:
    print("\nOperation stopped by user.")

finally:
    if ser.is_open:
        ser.close()
        print("Serial port closed.")
