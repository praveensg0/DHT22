import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import re

# Read log file
log_file = "dht_log.txt"

# Extract data from log file
timestamps, temperatures, humidities = [], [], []
cycle_offset = 0  # To adjust time discontinuities

with open(log_file, "r") as file:
    for line in file:
        match = re.search(r"Time\s*:\s*(\d+),\s*Temp\s*:\s*([\d.]+),\s*Hum\s*:\s*([\d.]+)", line)
        if match:
            time = int(match.group(1))
            temp = float(match.group(2))
            hum = float(match.group(3))

            # If time resets to 0, add cycle offset
            if time == 0 and timestamps:
                cycle_offset = timestamps[-1] + 30  # Assuming 30 sec intervals

            timestamps.append(time + cycle_offset)
            temperatures.append(temp)
            humidities.append(hum)

# Convert timestamps to hours for better readability
time_hours = [t / 3600 for t in timestamps]

# Create DataFrame
df = pd.DataFrame({"Time (hours)": time_hours, "Temperature (°C)": temperatures, "Humidity (%)": humidities})

# Use seaborn theme for cleaner plots 
sns.set_style("whitegrid")
sns.set_context("talk")

# Plot Temperature vs Time
plt.figure(figsize=(10, 5))
sns.lineplot(x=df["Time (hours)"], y=df["Temperature (°C)"], color="red", linewidth=2.5)
plt.xlabel("Time (hours)", fontsize=14)
plt.ylabel("Temperature (°C)", fontsize=14)
plt.title("Temperature Over Time", fontsize=16, fontweight="bold")
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.grid(True, linestyle="--", alpha=0.7)
plt.show()

# Plot Humidity vs Time
plt.figure(figsize=(10, 5))
sns.lineplot(x=df["Time (hours)"], y=df["Humidity (%)"], color="blue", linewidth=2.5)
plt.xlabel("Time (hours)", fontsize=14)
plt.ylabel("Humidity (%)", fontsize=14)
plt.title("Humidity Over Time", fontsize=16, fontweight="bold")
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.grid(True, linestyle="--", alpha=0.7)
plt.show()

# Print basic analytics
print("Summary Statistics:")
print(df.describe())
