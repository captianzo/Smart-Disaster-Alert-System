#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NONE 0
#define LOW 1
#define MODERATE 2
#define HIGH 3
#define MAX_RECORDS 10
#define MAX_PRECAUTIONS 3
#define MAX_PREDICTIONS 5

typedef struct
{
	char precautions[MAX_PRECAUTIONS][200];
	char predictions[MAX_PREDICTIONS][200];
	int precautionCount;
	int predictionCount;
} DisasterInfo;

typedef struct
{
	int disasterType; // 1=Earthquake, 2=Flood, 3=Cyclone, 4=Fire
	int severityLevel;
	char timestamp[50];
	float primaryMeasure;	// magnitude, water level, wind speed, temperature
	float secondaryMeasure; // only used for fire (smoke level)
	DisasterInfo info;
} DisasterRecord;

DisasterRecord history[MAX_RECORDS];
int recordCount = 0;

// Get text for severity level and print severity message
const char *getSeverityText(int level)
{
	const char *texts[] = {"No threat", "Low risk", "Moderate threat", "HIGH ALERT"};
	return (level >= 0 && level <= 3) ? texts[level] : "Unknown";
}

void printSeverity(int level)
{
	switch (level)
	{
	case NONE:
		printf("\033[1;32mStatus: No threat detected.\033[0m\n");
		break;
	case LOW:
		printf("\033[1;33mStatus: Low-level risk. Stay alert.\033[0m\n");
		break;
	case MODERATE:
		printf("\033[1;35mStatus: Moderate threat. Take precautions.\033[0m\n");
		break;
	case HIGH:
		printf("\033[1;31mStatus: HIGH ALERT! EMERGENCY PROTOCOLS ACTIVATED!\033[0m\n");
		break;
	default:
		printf("\033[1;37mStatus: Unknown\033[0m\n");
		break;
	}
}

// Print precautions and predictions based on severity
void printDisasterInfo(DisasterInfo info, int severity)
{
	if (severity == NONE)
		return;

	printf("\n\033[1;34m--- RECOMMENDED PRECAUTIONS ---\033[0m\n");
	for (int i = 0; i < info.precautionCount; i++)
		printf("\033[1;33m%d.\033[0m %s\n", i + 1, info.precautions[i]);

	if (severity >= LOW)
	{
		printf("\n\033[1;34m--- RESEARCH-BASED PREDICTIONS ---\033[0m\n");
		for (int i = 0; i < info.predictionCount; i++)
			printf("\033[1;36m%d.\033[0m %s\n", i + 1, info.predictions[i]);
	}
}

// Get current timestamp
void getCurrentTimestamp(char *timestamp)
{
	time_t now = time(NULL);
	// Gets the current time from system in seconds
	struct tm *t = localtime(&now);
	// Converts raw time from seconds into a struct 'tm' which breaks it down into year, month, day, hour, minute etc.
	strftime(timestamp, 50, "%a %d %B %Y %H:%M:%S", t);
	// strftime will format broken down time 't' into human readable string, that will be stored in 'timestamp'
}

// Store disaster record
void storeRecord(int type, int severity, float primary, float secondary, DisasterInfo info)
{
	if (recordCount >= MAX_RECORDS)
	{
		for (int i = 0; i < MAX_RECORDS - 1; i++)
			history[i] = history[i + 1];
		recordCount = MAX_RECORDS - 1;
	}
	// Oldest record will be deleted and new record will be updated at the last index

	DisasterRecord record;
	record.disasterType = type;
	record.severityLevel = severity;
	record.primaryMeasure = primary;
	record.secondaryMeasure = secondary;
	record.info = info;
	getCurrentTimestamp(record.timestamp);

	history[recordCount++] = record;
}

// Show disaster history
void showDisasterHistory()
{
	if (recordCount == 0)
	{
		printf("No disaster records available.\n");
		return;
	}

	printf("\n=== DISASTER PREDICTION HISTORY ===\n");
	printf("Recent predictions (%d):\n", recordCount);

	for (int i = 0; i < recordCount; i++)
	{
		DisasterRecord record = history[i];
		printf("\n#%d - ", i + 1);

		printf("\033[1;36m");

		switch (record.disasterType)
		{
		case 1:
			printf("Earthquake (Magnitude: %.1f)", record.primaryMeasure);
			break;
		case 2:
			printf("Flood (Water Level: %.1f m)", record.primaryMeasure);
			break;
		case 3:
			printf("Cyclone (Wind Speed: %.0f km/h)", record.primaryMeasure);
			break;
		case 4:
			printf("Fire (Temp: %.1f°C, Smoke: %.0f%%)", record.primaryMeasure, record.secondaryMeasure);
			break;
		}
		printf("\033[0m");

		int level = record.severityLevel;
		const char *color;
		switch (level)
		{
		case NONE:
			color = "\033[1;32m";
			break;
		case LOW:
			color = "\033[1;33m";
			break;
		case MODERATE:
			color = "\033[1;35m";
			break;
		case HIGH:
			color = "\033[1;31m";
			break;
		default:
			color = "\033[0m";
			break;
		}
		printf(" - %s%s\033[0m\n", color, getSeverityText(level));

		printf("\033[2;37m   Date & Time: %s\033[0m\n", record.timestamp);
	}
}

// Initialize disaster info with research-based predictions
DisasterInfo initDisasterInfo(int type)
{
	DisasterInfo info = {{0}, {0}, 3, 5};

	switch (type)
	{
	case 1: // Earthquake
		strcpy(info.precautions[0], "Drop, cover, and hold on. Stay away from windows.");
		strcpy(info.precautions[1], "If outdoors, move to open areas away from buildings and power lines.");
		strcpy(info.precautions[2], "Prepare for aftershocks which can occur minutes to months later.");

		strcpy(info.predictions[0], "8 out of 10 chance of smaller quakes occurring within the next 24 hours.");
		strcpy(info.predictions[1], "Areas with steep hills have 2.5 times higher chance of landslides.");
		strcpy(info.predictions[2], "Cell phone service will likely drop to 30-60% of normal capacity for several hours.");
		strcpy(info.predictions[3], "Power will likely be restored within 1-3 days in city areas based on past events.");
		strcpy(info.predictions[4], "Aftershocks will decrease in frequency over time, with most occurring in first week.");
		break;

	case 2: // Flood
		strcpy(info.precautions[0], "Move to higher ground immediately; 6 inches of rushing water can knock you down.");
		strcpy(info.precautions[1], "Do not walk or drive through flood waters - 1 foot of water can float vehicles.");
		strcpy(info.precautions[2], "Prepare clean water supplies; tap water may be unsafe for several days.");

		strcpy(info.predictions[0], "Flood waters typically reach their highest point 3-6 hours after rain stops.");
		strcpy(info.predictions[1], "Water levels will drop about 4-12 inches per hour once flooding peaks.");
		strcpy(info.predictions[2], "Nearly 9 out of 10 flooded areas will have unsafe drinking water with harmful bacteria.");
		strcpy(info.predictions[3], "Roads have a 40% higher chance of damage after being underwater for more than 24 hours.");
		strcpy(info.predictions[4], "Mold will begin growing in flooded buildings within 1-2 days.");
		break;

	case 3: // Cyclone
		strcpy(info.precautions[0], "Shelter in a room without windows on the lowest floor.");
		strcpy(info.precautions[1], "Prepare enough food and water for at least 3 days without utilities.");
		strcpy(info.precautions[2], "Secure loose objects; most injuries happen from flying debris.");

		strcpy(info.predictions[0], "The most dangerous flooding usually arrives 5-6 hours before the storm center.");
		strcpy(info.predictions[1], "Wind speeds will drop by about half within 60 miles of moving inland.");
		strcpy(info.predictions[2], "The heaviest rain typically falls 2-4 hours after the storm center passes.");
		strcpy(info.predictions[3], "About 6 out of 10 homes will get power back within 24 hours, 8 out of 10 within 3 days.");
		strcpy(info.predictions[4], "Risk of disease outbreaks is 6 times higher in areas without clean water after storms.");
		break;

	case 4: // Fire
		strcpy(info.precautions[0], "Evacuate immediately if authorities recommend it.");
		strcpy(info.precautions[1], "Cover nose/mouth with wet cloth; stay low in smoke-filled areas.");
		strcpy(info.precautions[2], "Close all doors and windows to keep smoke from entering buildings.");

		strcpy(info.predictions[0], "Forest fires typically spread at 7-9 miles per hour with wind, and twice as fast uphill.");
		strcpy(info.predictions[1], "Dangerous levels of toxic gases build up within 15-30 minutes in closed spaces.");
		strcpy(info.predictions[2], "When smoke reaches 35% density, visibility drops to less than 30 feet.");
		strcpy(info.predictions[3], "95% of building fires become too large to control after 8-10 minutes.");
		strcpy(info.predictions[4], "Areas that burned have 10 times higher risk of mudslides when it rains afterwards.");
		break;
	}
	return info;
}

// Detection functions
int detectDisaster(int type, float primary, float secondary)
{
	switch (type)
	{
	case 1: // Earthquake
		if (primary >= 6.0)
			return HIGH;
		if (primary >= 4.0)
			return MODERATE;
		if (primary >= 2.5)
			return LOW;
		break;
	case 2: // Flood
		if (primary >= 3.0)
			return HIGH;
		if (primary >= 1.5)
			return MODERATE;
		if (primary >= 1.0)
			return LOW;
		break;
	case 3: // Cyclone
		if (primary >= 120)
			return HIGH;
		if (primary >= 80)
			return MODERATE;
		if (primary >= 60)
			return LOW;
		break;
	case 4: // Fire
		if (primary >= 60 && secondary >= 50)
			return HIGH;
		if (primary >= 45 || secondary >= 40)
			return MODERATE;
		if (primary >= 35)
			return LOW;
		break;
	}
	return NONE;
}

// Main function
int main()
{
	int choice, result, continueProgram = 1;
	const char *types[] = {"", "Earthquake", "Flood", "Cyclone", "Fire"};
	const char *units[] = {"", "Magnitude (Richter)", "Water Level (m)",
						   "Wind Speed (km/h)", "Temperature (°C) & Smoke Level (0-100%)"};
	float primary, secondary;

	printf("\n    **************************************************\n");
	printf("    *       SMART DISASTER ALERT SYSTEM              *\n");
	printf("    *       Emergency Prediction & Response          *\n");
	printf("    **************************************************\n\n");

	while (continueProgram)
	{
		printf("\n=== SMART DISASTER DETECTION SYSTEM ===\n");
		printf("1. Earthquake\n2. Flood\n3. Cyclone\n4. Fire\n");
		printf("5. View Prediction History\n6. Exit System\n");
		printf("Enter choice (1-6): ");
		scanf("%d", &choice);

		if (choice >= 1 && choice <= 4)
		{
			printf("\n--- Disaster Detection Report ---\n");
			printf("Type: %s | Unit: %s\n", types[choice], units[choice]);

			DisasterInfo info = initDisasterInfo(choice);
			secondary = 0;

			if (choice == 4)
			{
				printf("Enter temperature: ");
				scanf("%f", &primary);
				printf("Enter smoke level: ");
				scanf("%f", &secondary);
			}
			else
			{
				printf("Enter %s: ", units[choice]);
				scanf("%f", &primary);
			}

			result = detectDisaster(choice, primary, secondary);
			printSeverity(result);
			printDisasterInfo(info, result);
			storeRecord(choice, result, primary, secondary, info);
		}
		else if (choice == 5)
		{
			showDisasterHistory();
		}
		else if (choice == 6)
		{
			printf("Exiting Smart Disaster Detection System. Stay safe!\n");
			break;
		}
		else
		{
			printf("Invalid choice. Please select 1-6.\n");
		}

		if (choice != 6)
		{
			printf("\nReturn to main menu? (y/n): ");
			char response;
			scanf(" %c", &response);
			if (response != 'y' && response != 'Y')
				break;
		}
	}
	return 0;
}