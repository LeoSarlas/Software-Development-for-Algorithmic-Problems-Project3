import re

# Define a regular expression pattern to extract relevant information from each log entry
pattern = re.compile(r"Validation Loss \(conv_layers = (\d+), filter_size = (\d+), filters_per_layer = (\d+), epochs=(\d+), batch_size=(\d+)\): ([\d.]+)")

# Function to parse a single log entry and extract information
def parse_log_entry(entry):
    match = pattern.search(entry)
    if match:
        conv_layers, filter_size, filters_per_layer, epochs, batch_size, validation_loss = map(float, match.groups())
        return {'conv_layers': int(conv_layers), 'filter_size': int(filter_size),
                'filters_per_layer': int(filters_per_layer), 'epochs': int(epochs),
                'batch_size': int(batch_size), 'validation_loss': validation_loss}
    return None

# Read the log file
with open('log.txt', 'r') as file:
    log_content = file.read()

# Split the log content into individual log entries
log_entries = log_content.split("Epoch ")

# Parse each log entry and store the results
results = [parse_log_entry("Validation Loss" + entry) for entry in log_entries if "Validation Loss" in entry]

# Remove any None entries (entries that couldn't be parsed)
results = [entry for entry in results if entry is not None]

# Sort the results based on validation loss
results.sort(key=lambda x: x['validation_loss'])

# Print the 5 smallest validation losses
print("Top 5 combinations with smallest validation losses:")
for i, result in enumerate(results[:5], start=1):
    print(f"{i}. Conv Layers: {result['conv_layers']}, Filter Size: {result['filter_size']}, "
          f"Filters per Layer: {result['filters_per_layer']}, Epochs: {result['epochs']}, "
          f"Batch Size: {result['batch_size']}, Validation Loss: {result['validation_loss']}")
