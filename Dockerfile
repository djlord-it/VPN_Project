# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set the working directory
WORKDIR /app

# Copy project files into the container
COPY . .

# Expose the necessary port (if applicable)
EXPOSE 8080

# Default command to keep the container running
CMD ["/bin/bash"]
