# .devcontainer/Dockerfile
FROM islandc/armcm-devcontainer:latest

# Install vendor tools, SDK, etc.
RUN apt-get update && \
    apt-get install -y vendor-tool-x && \
    rm -rf /var/lib/apt/lists/*
