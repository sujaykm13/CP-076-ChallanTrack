# CP-076 ChallanTrack: Traffic Violation and Penalty Management System

## Team
Srinivas K A ([Insert USN 1]), [Partner Name] ([Insert USN 2]), C track

## How to run
```bash
gcc main.c -o challantrack
./challantrack
```
*(On Windows Command Prompt or PowerShell, run `.\challantrack.exe` after compiling).*

## Features that work
1. **Maintain offences with fine amounts (Core Feature 1):** Preloads 4 standard traffic offences and allows officers to add new offences while blocking duplicate offence codes and zero/negative fine amounts.
2. **Issue a challan after checking vehicle number format (Core Feature 2):** Validates that the vehicle registration number matches the 10-character RTO format (`LLDDLLDDDD`, e.g., `KA01AB1234`) and automatically normalizes lowercase input to uppercase.
3. **Increase the fine for repeat offences by the same vehicle (Core Feature 3):** Scans historical records for the same vehicle number and automatically adds 50% of the base fine for every previous offence.
4. **Record payment of a challan (Core Feature 4):** Looks up a challan by its unique ID, marks it as paid, and blocks duplicate payments on already-paid tickets.
5. **List pending challans by vehicle and total amount due (Core Feature 5):** Displays all unpaid tickets for a given vehicle number and calculates the total pending fine amount.
6. **Suspend a licence after a set number of offences (Stretch Feature 1):** Automatically triggers an urgent licence suspension alert when a vehicle reaches 3 or more offences.
7. **Report the most common offences by area (Stretch Feature 2):** Tallies violations recorded in a specific area and highlights the most frequent offence.
8. **Persistent file storage (Stretch Feature 3):** Automatically saves and reloads all challans using `database.txt`.

## Sample run
```text
================================================
 CP-076 CHALLANTRACK: TRAFFIC PENALTY SYSTEM
================================================
1. Maintain Offences (View / Add Offences)
2. Issue New Challan
3. Record Challan Payment
4. List Pending Challans by Vehicle
5. Area Offence Report (Stretch Feature)
6. Exit
Enter your choice (1-6): 2

--- 2. Issue New Challan ---
Enter Vehicle Number (Format: KA01AB1234): ka07ab1234
Enter Area of Offence (e.g., MG_Road, Indiranagar): MG_Road

Available Offences:
  [1] Riding Without Helmet (Base Fine: Rs. 500)
  [2] Signal Jumping (Base Fine: Rs. 1000)
  [3] Over Speeding (Base Fine: Rs. 1500)
  [4] Driving Without Valid Licence (Base Fine: Rs. 2000)
Select Offence Code: 2

================================================
CHALLAN ISSUED SUCCESSFULLY
Challan ID     : 1001
Vehicle Number : KA07AB1234
Area           : MG_ROAD
Offence        : Signal Jumping
Total Fine     : Rs. 1000 (Status: PENDING)
================================================

Enter your choice (1-6): 2

--- 2. Issue New Challan ---
Enter Vehicle Number (Format: KA01AB1234): KA07AB1234
Enter Area of Offence (e.g., MG_Road, Indiranagar): MG_Road
Select Offence Code: 2

================================================
CHALLAN ISSUED SUCCESSFULLY
Challan ID     : 1002
Vehicle Number : KA07AB1234
Area           : MG_ROAD
Offence        : Signal Jumping
Repeat Notice  : 1 prior offence(s) found! Fine increased.
Total Fine     : Rs. 1500 (Status: PENDING)
================================================

Enter your choice (1-6): 4

--- 4. List Pending Challans by Vehicle ---
Enter Vehicle Number: KA07AB1234

Pending Challans for Vehicle: KA07AB1234
------------------------------------------------------------------------
ID      Fine (Rs)       Area            Offence Description
------------------------------------------------------------------------
1001    Rs. 1000        MG_ROAD         Signal Jumping
1002    Rs. 1500        MG_ROAD         Signal Jumping
------------------------------------------------------------------------
Total Pending Tickets : 2
TOTAL AMOUNT DUE      : Rs. 2500
```
