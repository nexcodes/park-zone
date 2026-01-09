# Smart Parking Management System - Build Script

# Compile all source files
Write-Host "Compiling Smart Parking Management System..." -ForegroundColor Green

# Create output directory
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# Compilation command
$sources = @(
    "src/StateValidator.cpp",
    "src/Stack.cpp",
    "src/ParkingSlot.cpp",
    "src/ParkingArea.cpp",
    "src/Zone.cpp",
    "src/Vehicle.cpp",
    "src/ParkingRequest.cpp",
    "src/RollbackManager.cpp",
    "src/AllocationEngine.cpp",
    "src/ParkingSystem.cpp",
    "src/main.cpp"
)

$output = "build/parking_system.exe"
$includes = "-Iinclude"

Write-Host "Compiling with g++..." -ForegroundColor Yellow
$command = "g++ -std=c++17 $includes $($sources -join ' ') -o $output"

try {
    Invoke-Expression $command
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`nCompilation successful!" -ForegroundColor Green
        Write-Host "Executable: $output" -ForegroundColor Cyan
        Write-Host "`nRun with: .\build\parking_system.exe" -ForegroundColor Cyan
    } else {
        Write-Host "`nCompilation failed!" -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "Error during compilation: $_" -ForegroundColor Red
    exit 1
}
