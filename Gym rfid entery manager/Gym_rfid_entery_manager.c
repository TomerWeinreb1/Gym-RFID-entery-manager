#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <time.h>
#include "Gym_rfid_entery_manager.h"
#include <rs232.h>
#include <stdio.h>

static int panelHandle;
static int graphPanelHandle; // Handle for the panel with the graph
#define FILE_PATH "customer_data.txt"
#define MAX_CUSTOMERS 100 // Maximum number of customers

// COM port settings
#define COM_PORT 3
#define BAUD_RATE 9600

// Buffer to store data from Arduino
char readBuffer[100];
int bytesRead = 0;
int entryCount = 0; // Counter for tracking the number of entries
double xValues[1000]; // Array to store time values (X-axis)
double yValues[1000]; // Array to store entry count values (Y-axis)
int dataIndex = 0; // Index for storing data points

time_t startTime; // Store the start time of the program

// Function to save customer data to a text file
void SaveCustomerDataToFile(const char *name, int age, const char *address) {
    FILE *file = fopen(FILE_PATH, "w");
    if (file != NULL) {
        fprintf(file, "Name: %s\n", name);
        fprintf(file, "Age: %d\n", age);
        fprintf(file, "Address: %s\n", address);
        fclose(file);
    } 
}

// Function to get the current time in hours since the program started
double GetCurrentTimeInHours() {
    time_t currentTime = time(NULL);
    return difftime(currentTime, startTime) / 3600.0; // Convert seconds to hours
}

// Function to get the current date and time as a string
void GetCurrentTime(char *buffer, int bufferSize) {
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", tmp);
}

// Function to load customer data from the file and display it along with the current date and time
void LoadCustomerDataToDisplay() {
    FILE *file = fopen(FILE_PATH, "r");
    if (file != NULL) {
        char line[256];
        char displayData[1024] = ""; // Buffer to hold all the text for display
        char dateTime[20];

        GetCurrentTime(dateTime, sizeof(dateTime)); // Get the current date and time

        strcat(displayData, "Date and Time: ");
        strcat(displayData, dateTime);
        strcat(displayData, "\n");

        while (fgets(line, sizeof(line), file)) {
            strcat(displayData, line); // Concatenate each line to the display buffer
        }

        fclose(file);


        SetCtrlVal(panelHandle, PANEL_TXT_DISPLAY, displayData);
    }
}

// Function to update the customer entry count and plot it
void UpdateEntryCountAndPlot() {
    entryCount++; // Increment the entry count

    // Get the current time in hours since the program started
    double currentTime = GetCurrentTimeInHours();

    // Store the time and entry count in arrays
    xValues[dataIndex] = currentTime;
    yValues[dataIndex] = entryCount;
    dataIndex++;

    // Plot the data as a continuous line
    DeleteGraphPlot(graphPanelHandle, PANEL_2_GRAPH, -1, VAL_IMMEDIATE_DRAW); // Clear previous plot
    PlotXY(graphPanelHandle, PANEL_2_GRAPH, xValues, yValues, dataIndex, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
}

// Callback function to handle the "Send New" button click
int CVICALLBACK OnSendNew(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        // Get the new customer name from the input control
        char newName[50];
        GetCtrlVal(panelHandle, PANEL_NEW_NAME, newName);

        int newAge;
        GetCtrlVal(panelHandle, PANEL_NEW_AGE, &newAge);

        char newAddress[100];
        GetCtrlVal(panelHandle, PANEL_ADDRESS, newAddress);

        // Save the new customer data to the file
        SaveCustomerDataToFile(newName, newAge, newAddress);
        SetCtrlVal(panelHandle, PANEL_TXT_DISPLAY, "Please scan your card\n");
    }
    return 0;
}

// Function to read data from Arduino
void ReadFromArduino() {
    while (GetInQLen(COM_PORT) > 0) {
        bytesRead = ComRdTerm(COM_PORT, readBuffer, sizeof(readBuffer) - 1, '\n'); // Read until newline
        if (bytesRead > 0) {
            readBuffer[bytesRead] = '\0'; // Null-terminate the string

            if (strcmp(readBuffer, "Customer Not Recognized") == 1) {
                LoadCustomerDataToDisplay(); // Load and display the data after the RFID scan
                UpdateEntryCountAndPlot(); // Update the entry count and plot it on the graph as a continuous line
            }
        }
    }
}

// Callback for timer 
int CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_TIMER_TICK) {
        ReadFromArduino(); // Read data when the timer ticks
    }
    return 0;
}

// Callback function for the GRAPH button
int CVICALLBACK graph_but(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        // Hide the main panel
        HidePanel(panelHandle);
        // Show the graph panel
        DisplayPanel(graphPanelHandle);
    }
    return 0;
}

// Function to export data to a CSV file
void ExportToCSV(const char *filePath, const char *textData) {
    FILE *file = fopen(filePath, "a"); // Open the file in append mode
    if (file != NULL) {
        if (ftell(file) == 0) { // Write headers only if the file is empty
            fprintf(file, "Customer Data\n");
        }
        fprintf(file, "%s\n", textData); // Write the text data to the CSV file
        fclose(file);
        MessagePopup("Export", "Data successfully exported to CSV.");
    } else {
        MessagePopup("Error", "Failed to open the file for writing.");
    }
}

// Function to save and export customer data
void SaveAndExportCustomerData() {
    char textData[2048]; // Buffer to hold the text data from TXT_DISPLAY
    GetCtrlVal(panelHandle, PANEL_TXT_DISPLAY, textData);
    printf("TXT_DISPLAY data=%s\n", textData); // For debugging: Print the data
    ExportToCSV("customer_data.csv", textData); // Save the data to a CSV file
}

// Callback function for the Export button
int CVICALLBACK Export(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        SaveAndExportCustomerData(); // Save and export the customer data
    }
    return 0;
}

//quit panel 1
int CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        QuitUserInterface(0);
    }
    return 0;
}
//quit panel 2
int CVICALLBACK QuitCallback1 (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
        QuitUserInterface(0);
			break;
	}
	return 0;
}

int main (int argc, char *argv[]) {
    if (InitCVIRTE (0, argv, 0) == 0) {
        return -1; 
    }
    if ((panelHandle = LoadPanel (0, "Gym_rfid_entery_manager.uir", PANEL)) < 0) {
        return -1; 
    }
    if ((graphPanelHandle = LoadPanel (0, "Gym_rfid_entery_manager.uir", PANEL_2)) < 0) {
        return -1; 
    }

    startTime = time(NULL);
    SetCtrlVal(panelHandle, PANEL_TXT_DISPLAY, "Please scan your card\n");

    if (OpenComConfig(COM_PORT, "", BAUD_RATE, 0, 8, 1, 512, 512) < 0) {
        MessagePopup("Error", "Failed to open COM port 3.");
        return -1;
    }


    DisplayPanel(panelHandle);


    RunUserInterface();


    CloseCom(COM_PORT);
    DiscardPanel(panelHandle);
    DiscardPanel(graphPanelHandle);
    
    return 0;
}


