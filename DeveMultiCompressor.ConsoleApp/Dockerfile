#See https://aka.ms/containerfastmode to understand how Visual Studio uses this Dockerfile to build your images for faster debugging.

FROM mcr.microsoft.com/dotnet/runtime:8.0 AS base
# Install wine
RUN dpkg --add-architecture i386 && \
	apt-get update && \
    apt install wine wine32 wine64 libwine libwine:i386 fonts-wine -y

WORKDIR /app

FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build
WORKDIR /src
COPY ["DeveMultiCompressor.ConsoleApp/DeveMultiCompressor.ConsoleApp.csproj", "DeveMultiCompressor.ConsoleApp/"]
COPY ["DeveMultiCompressor/DeveMultiCompressor.csproj", "DeveMultiCompressor/"]
RUN dotnet restore "DeveMultiCompressor.ConsoleApp/DeveMultiCompressor.ConsoleApp.csproj"
COPY . .
WORKDIR "/src/DeveMultiCompressor.ConsoleApp"
RUN dotnet build "DeveMultiCompressor.ConsoleApp.csproj" -c Release -o /app/build

FROM build AS publish
ARG BUILD_VERSION
ARG VER=${BUILD_VERSION:-1.0.0}
RUN dotnet publish "DeveMultiCompressor.ConsoleApp.csproj" -c Release -o /app/publish /p:Version=$VER

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
# Make executable files without extension executable
RUN echo "Making executable files without extension executable..." && \
    find . -type f ! -name "*.*" -exec sh -c '\
        for f; do \
            mime=$(file --mime-type "$f" | cut -d " " -f2); \
            if [ "$mime" = "application/x-pie-executable" ] || [ "$mime" = "application/x-executable" ]; then \
                chmod +x "$f"; \
            fi; \
        done \
    ' sh {} +
ENTRYPOINT ["dotnet", "DeveMultiCompressor.ConsoleApp.dll"]
