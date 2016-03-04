#include "opcn2-library.h"

Opcn2Library::OPCN2::OPCN2(uint8_t chip_select){
    _CS = chip_select;

    // Set up SPI
    SPI.begin(_CS);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1);
    SPI.setClockSpeed(1000000);

    // Set the firmware version by reading the info String
    firmware_version = this->info_string();
}
/*
uint16_t Opcn2Library::OPCN2::_16bit_int(byte LSB, byte MSB){
    // Combine two bytes into a 16-bit unsigned int
    return ((MSB << 8) | LSB);
}
*/
bool Opcn2Library::OPCN2::_compare_arrays(byte array1[], byte array2[], int length){
    // Compare two arrays
    bool result = true;

    for (int i = 0; i < length; i++){
        if (array1[i] != array2[i]){
            result = false;
        }
    }

    return result;
}

/*
float Opcn2Library::OPCN2::_calculate_float(byte val0, byte val1, byte val2, byte val3){
    // Return an IEEE754 float from an array of 4 bytes
    union u_tag {
        byte b[4];
        float val;
    } u;

    u.b[0] = val0;
    u.b[1] = val1;
    u.b[2] = val2;
    u.b[3] = val3;

    return u.val;
}

uint32_t Opcn2Library::OPCN2::_32bit_int(byte val0, byte val1, byte val2, byte val3){
    // Return a 32-bit unsigned int from 4 bytes
    return ((val3 << 24) | (val2 << 16) | (val1 << 8) | val0);
}

*/

bool Opcn2Library::OPCN2::ping(){
    // Isse the check status command
    byte resp[1];
    byte expected[] = {0xF3};

    digitalWrite(this->_CS, LOW);       // pull the pin low
    resp[0] = SPI.transfer(0xCF);       // issue the command byte
    digitalWrite(this->_CS, HIGH);      // pull the pin high

    return this->_compare_arrays(resp, expected, 1);
}

bool Opcn2Library::OPCN2::on(){
    // Turn ON the OPC
    byte vals[2];
    byte expected[] = {0xF3, 0x03};

    digitalWrite(this->_CS, LOW);
    vals[0] = SPI.transfer(0x03);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    vals[1] = SPI.transfer(0x00);
    digitalWrite(this->_CS, HIGH);

    return this->_compare_arrays(vals, expected, 2);
}

bool Opcn2Library::OPCN2::off(){
    // Turn OFF the OPC
    byte vals[2];
    byte expected[] = {0xF3, 0x03};

    digitalWrite(this->_CS, LOW);
    vals[0] = SPI.transfer(0x03);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    vals[1] = SPI.transfer(0x01);
    digitalWrite(this->_CS, HIGH);

    return this->_compare_arrays(vals, expected, 2);
}
/*
bool Opcn2Library::OPCN2::write_config_variables(byte values[]){
    // Write the configuration [NOT IMPLEMENTED]

    return true;
}

bool Opcn2Library::OPCN2::save_config_variables(){
    // Save the config variables
    byte resp[6];
    byte commands[] = {0x3F, 0x3C, 0x3F, 0x3C, 0x43};
    byte expected[] = {0xF3, 0x43, 0x3f, 0x3c, 0x3f, 0x3c};

    digitalWrite(this->_CS, LOW);
    resp[0] = SPI.transfer(0x43);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    for (int i = 0; i < 5; i++){
        resp[i + 1] = SPI.transfer(commands[i]);
        delayMicroseconds(4);
    }

    digitalWrite(this->_CS, HIGH);      // Pull the pin high

    return this->_compare_arrays(resp, expected, 6);
}

void Opcn2Library::OPCN2::enter_bootloader(){
    // Enter bootloader mode
    byte resp[1];
    byte expected[] = {0xF3};

    digitalWrite(this->_CS, LOW);
    resp[0] = SPI.transfer(0x41);
    digitalWrite(this->_CS, HIGH);

    return;
}

void Opcn2Library::OPCN2::set_fan_power(uint8_t value){
    // Set the Fan Power
    byte resp[3];
    byte expected[] = {0xF3, 0x42, 0x00};

    digitalWrite(this->_CS, LOW);
    resp[0] = SPI.transfer(0x42);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    resp[1] = SPI.transfer(0x00);

    delayMicroseconds(4);

    resp[2] = SPI.transfer(value);
    digitalWrite(this->_CS, HIGH);

    return;
}

void Opcn2Library::OPCN2::set_laser_power(uint8_t value){
    // Set the Laser Power
    byte resp[3];
    byte expected[] = {0xF3, 0x42, 0x01};

    digitalWrite(this->_CS, LOW);
    resp[0] = SPI.transfer(0x42);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    resp[1] = SPI.transfer(0x01);

    delayMicroseconds(4);

    resp[2] = SPI.transfer(value);
    digitalWrite(this->_CS, HIGH);

    return;
}

void Opcn2Library::OPCN2::toggle_fan(bool state){
    // Toggle the power state of the fan
    byte resp[2];
    byte expected[] = {0xF3, 0x03};

    digitalWrite(this->_CS, LOW);
    resp[0] = SPI.transfer(0x03);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    // turn either on or off
    digitalWrite(this->_CS, LOW);
    if (state == true){
        resp[1] = SPI.transfer(0x04);
    }
    else {
        resp[1] = SPI.transfer(0x05);
    }

    digitalWrite(this->_CS, HIGH);

    return;
}

void Opcn2Library::OPCN2::toggle_laser(bool state){
    // Toggle the power state of the laser
    byte resp[2];
    byte expected[] = {0xF3, 0x03};

    digitalWrite(this->_CS, LOW);
    resp[0] = SPI.transfer(0x03);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    if (state == true){
        resp[1] = SPI.transfer(0x02);
    }
    else {
        resp[1] = SPI.transfer(0x03);
    }

    digitalWrite(this->_CS, HIGH);

    return;
}

histogram_data Opcn2Library::OPCN2::histogram(){
    // Read the histogram
    histogram_data data;                // Empty structure for data
    byte vals[62];                      // Empty array of type bytes

    // Read the data and clear the local memory
    digitalWrite(this->_CS, LOW);       // Pull the CS Low
    SPI.transfer(0x30);                 // Transfer the command byte
    digitalWrite(this->_CS, HIGH);

    delay(12);                          // Delay for 12 ms

    // Send commands and build array of data
    digitalWrite(this->_CS, LOW);

    for (int i = 0; i < 62; i++){
        vals[i] = SPI.transfer(0x30);
        delayMicroseconds(4);
    }

    digitalWrite(this->_CS, HIGH);      // Pull the CS High

    // Calculate all of the values!
    data.bin0 = this->_16bit_int(vals[0], vals[1]);
    data.bin1 = this->_16bit_int(vals[2], vals[3]);
    data.bin2 = this->_16bit_int(vals[4], vals[5]);
    data.bin3 = this->_16bit_int(vals[6], vals[7]);
    data.bin4 = this->_16bit_int(vals[8], vals[9]);
    data.bin5 = this->_16bit_int(vals[10], vals[11]);
    data.bin6 = this->_16bit_int(vals[12], vals[13]);
    data.bin7 = this->_16bit_int(vals[14], vals[15]);
    data.bin8 = this->_16bit_int(vals[16], vals[17]);
    data.bin9 = this->_16bit_int(vals[18], vals[19]);
    data.bin10 = this->_16bit_int(vals[20], vals[21]);
    data.bin11 = this->_16bit_int(vals[22], vals[23]);
    data.bin12 = this->_16bit_int(vals[24], vals[25]);
    data.bin13 = this->_16bit_int(vals[26], vals[27]);
    data.bin14 = this->_16bit_int(vals[28], vals[29]);
    data.bin15 = this->_16bit_int(vals[30], vals[31]);

    data.bin1MToF = int(vals[32]) / 3.0;
    data.bin3MToF = int(vals[33]) / 3.0;
    data.bin5MToF = int(vals[34]) / 3.0;
    data.bin7MToF = int(vals[35]) / 3.0;

    data.sfr = this->_calculate_float(vals[36], vals[37], vals[38], vals[39]);

    // This holds either temperature or pressure
    // If temp, this is temp in C x 10
    // If pressure, this is pressure in Pa
    data.temp_pressure = this->_32bit_int(vals[40], vals[41], vals[42], vals[43]);

    data.period = this->_calculate_float(vals[44], vals[45], vals[46], vals[47]);

    data.checksum = this->_16bit_int(vals[48], vals[49]);

    data.pm1 = this->_calculate_float(vals[50], vals[51], vals[52], vals[53]);
    data.pm25 = this->_calculate_float(vals[54], vals[55], vals[56], vals[57]);
    data.pm10 = this->_calculate_float(vals[58], vals[59], vals[60], vals[61]);

    return data;
}

config_variables Opcn2Library::OPCN2::config(){
    // Read the config variables
    config_variables results;       // empty structure for the data
    byte vals[256];

    // Read the config variables
    digitalWrite(this->_CS, LOW);
    SPI.transfer(0x3c);
    digitalWrite(this->_CS, HIGH);

    delay(10);

    digitalWrite(this->_CS, LOW);
    for (int i = 0; i < 256; i++){
        vals[i] = SPI.transfer(0x00);
        delayMicroseconds(4);
    }

    digitalWrite(this->_CS, HIGH);

    // Fill in the results
    results.bb0 = this->_16bit_int(vals[0], vals[1]);
    results.bb1 = this->_16bit_int(vals[2], vals[3]);
    results.bb2 = this->_16bit_int(vals[4], vals[5]);
    results.bb3 = this->_16bit_int(vals[6], vals[7]);
    results.bb4 = this->_16bit_int(vals[8], vals[9]);
    results.bb5 = this->_16bit_int(vals[10], vals[11]);
    results.bb6 = this->_16bit_int(vals[12], vals[13]);
    results.bb7 = this->_16bit_int(vals[14], vals[15]);
    results.bb8 = this->_16bit_int(vals[16], vals[17]);
    results.bb9 = this->_16bit_int(vals[18], vals[19]);
    results.bb10 = this->_16bit_int(vals[20], vals[21]);
    results.bb11 = this->_16bit_int(vals[22], vals[23]);
    results.bb12 = this->_16bit_int(vals[24], vals[25]);
    results.bb13 = this->_16bit_int(vals[26], vals[27]);
    results.bb14 = this->_16bit_int(vals[28], vals[29]);

    // Bin Particle Volumes
    results.bpv0 = this->_calculate_float(vals[32], vals[33], vals[34], vals[35]);
    results.bpv1 = this->_calculate_float(vals[36], vals[37], vals[38], vals[39]);
    results.bpv2 = this->_calculate_float(vals[40], vals[41], vals[42], vals[43]);
    results.bpv3 = this->_calculate_float(vals[44], vals[45], vals[46], vals[47]);
    results.bpv4 = this->_calculate_float(vals[48], vals[49], vals[50], vals[51]);
    results.bpv5 = this->_calculate_float(vals[52], vals[53], vals[54], vals[55]);
    results.bpv6 = this->_calculate_float(vals[56], vals[57], vals[58], vals[59]);
    results.bpv7 = this->_calculate_float(vals[60], vals[61], vals[62], vals[63]);
    results.bpv8 = this->_calculate_float(vals[64], vals[65], vals[66], vals[67]);
    results.bpv9 = this->_calculate_float(vals[68], vals[69], vals[70], vals[71]);
    results.bpv10 = this->_calculate_float(vals[72], vals[73], vals[74], vals[75]);
    results.bpv11 = this->_calculate_float(vals[76], vals[77], vals[78], vals[79]);
    results.bpv12 = this->_calculate_float(vals[80], vals[81], vals[82], vals[83]);
    results.bpv13 = this->_calculate_float(vals[84], vals[85], vals[86], vals[87]);
    results.bpv14 = this->_calculate_float(vals[88], vals[89], vals[90], vals[91]);
    results.bpv15 = this->_calculate_float(vals[92], vals[93], vals[94], vals[95]);

    // Bin Particle Densities
    results.bpd0 = this->_calculate_float(vals[96], vals[97], vals[98], vals[99]);
    results.bpd1 = this->_calculate_float(vals[100], vals[101], vals[102], vals[103]);
    results.bpd2 = this->_calculate_float(vals[104], vals[105], vals[106], vals[107]);
    results.bpd3 = this->_calculate_float(vals[108], vals[109], vals[110], vals[111]);
    results.bpd4 = this->_calculate_float(vals[112], vals[113], vals[114], vals[115]);
    results.bpd5 = this->_calculate_float(vals[116], vals[117], vals[118], vals[119]);
    results.bpd6 = this->_calculate_float(vals[120], vals[121], vals[122], vals[123]);
    results.bpd7 = this->_calculate_float(vals[124], vals[125], vals[126], vals[127]);
    results.bpd8 = this->_calculate_float(vals[128], vals[129], vals[130], vals[131]);
    results.bpd9 = this->_calculate_float(vals[132], vals[133], vals[134], vals[135]);
    results.bpd10 = this->_calculate_float(vals[136], vals[137], vals[138], vals[139]);
    results.bpd11 = this->_calculate_float(vals[140], vals[141], vals[142], vals[143]);
    results.bpd12 = this->_calculate_float(vals[144], vals[145], vals[146], vals[147]);
    results.bpd13 = this->_calculate_float(vals[148], vals[149], vals[150], vals[151]);
    results.bpd14 = this->_calculate_float(vals[152], vals[153], vals[154], vals[155]);
    results.bpd15 = this->_calculate_float(vals[156], vals[157], vals[158], vals[159]);

    // Bin Particle Sample Volumes
    results.bsvw0 = this->_calculate_float(vals[160], vals[161], vals[162], vals[163]);
    results.bsvw1 = this->_calculate_float(vals[164], vals[165], vals[166], vals[167]);
    results.bsvw2 = this->_calculate_float(vals[168], vals[169], vals[170], vals[171]);
    results.bsvw3 = this->_calculate_float(vals[172], vals[173], vals[174], vals[175]);
    results.bsvw4 = this->_calculate_float(vals[176], vals[177], vals[178], vals[179]);
    results.bsvw5 = this->_calculate_float(vals[180], vals[181], vals[182], vals[183]);
    results.bsvw6 = this->_calculate_float(vals[184], vals[185], vals[186], vals[187]);
    results.bsvw7 = this->_calculate_float(vals[188], vals[189], vals[190], vals[191]);
    results.bsvw8 = this->_calculate_float(vals[192], vals[193], vals[194], vals[195]);
    results.bsvw9 = this->_calculate_float(vals[196], vals[197], vals[198], vals[199]);
    results.bsvw10 = this->_calculate_float(vals[200], vals[201], vals[202], vals[203]);
    results.bsvw11 = this->_calculate_float(vals[204], vals[205], vals[206], vals[207]);
    results.bsvw12 = this->_calculate_float(vals[208], vals[209], vals[210], vals[211]);
    results.bsvw13 = this->_calculate_float(vals[212], vals[213], vals[214], vals[215]);
    results.bsvw14 = this->_calculate_float(vals[216], vals[217], vals[218], vals[219]);
    results.bsvw15 = this->_calculate_float(vals[220], vals[221], vals[222], vals[223]);

    // Gain Scaling Coefficient
    results.gsc = this->_calculate_float(vals[224], vals[225], vals[226], vals[227]);

    // Sample Flow Rate (ml/s)
    results.sfr = this->_calculate_float(vals[228], vals[229], vals[230], vals[231]);

    // LaserDAC
    results.laser_dac = (unsigned int)vals[232];
    results.fan_dac = (unsigned int)vals[233];

    // Time-of-Flight to Sample Flow Rate ratio
    results.tof_sfr = (unsigned int)vals[234];

    return results;
}
*/
String Opcn2Library::OPCN2::info_string(){
    // Read the info String and return the firmware version
    String result = "";
    String tmp;

    // Return the information string
    byte vals[61];                      // dummy array of bytes

    digitalWrite(this->_CS, LOW);       // Pull the CS low
    SPI.transfer(0x3F);                 // Send the start command
    digitalWrite(this->_CS, HIGH);       // Pull the CS High

    delay(3);

    // Iterate to read the entire string
    digitalWrite(this->_CS, LOW);
    for (int i = 0; i < 61; i++){
        vals[i] = SPI.transfer(0x00);
        delayMicroseconds(4);

    }

    digitalWrite(this->_CS, HIGH);      // Pull the CS pin high again

    // Convert the array of bytes into a String
    for (int j = 0; j < 58; j++){
        tmp = (char)vals[j];
        result.concat(tmp);
    }

    // Isolate just the Firmware version
    result = String(result).replace(".", "").trim().substring(23);

    return result;
}
