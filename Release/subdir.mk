################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BidCoSMessage.cpp \
../BidCoSMessages.cpp \
../BidCoSPacket.cpp \
../BidCoSPacketManager.cpp \
../BidCoSQueue.cpp \
../BidCoSQueueManager.cpp \
../Cul.cpp \
../Database.cpp \
../Exception.cpp \
../GD.cpp \
../HelperFunctions.cpp \
../HomeMaticDevice.cpp \
../HomeMaticDevices.cpp \
../Log.cpp \
../Peer.cpp \
../PendingBidCoSQueues.cpp \
../ServiceMessages.cpp \
../Settings.cpp \
../main.cpp 

OBJS += \
./BidCoSMessage.o \
./BidCoSMessages.o \
./BidCoSPacket.o \
./BidCoSPacketManager.o \
./BidCoSQueue.o \
./BidCoSQueueManager.o \
./Cul.o \
./Database.o \
./Exception.o \
./GD.o \
./HelperFunctions.o \
./HomeMaticDevice.o \
./HomeMaticDevices.o \
./Log.o \
./Peer.o \
./PendingBidCoSQueues.o \
./ServiceMessages.o \
./Settings.o \
./main.o 

CPP_DEPS += \
./BidCoSMessage.d \
./BidCoSMessages.d \
./BidCoSPacket.d \
./BidCoSPacketManager.d \
./BidCoSQueue.d \
./BidCoSQueueManager.d \
./Cul.d \
./Database.d \
./Exception.d \
./GD.d \
./HelperFunctions.d \
./HomeMaticDevice.d \
./HomeMaticDevices.d \
./Log.d \
./Peer.d \
./PendingBidCoSQueues.d \
./ServiceMessages.d \
./Settings.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


