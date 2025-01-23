class ConfigEntry 
{
    public:
    // Konstruktor
    ConfigEntry(int id, int eeAddress) 
    {
      this->eeAddress = eeAddress;
      this->id = id;
    }

int getAddress(){return eeAddress;}

  protected:

    int eeAddress;
    int id; // stores the unique id from each Configitem, beginning at 1

};

class ConfigEntry_INT : public ConfigEntry
{
public:
    // Konstruktor
    ConfigEntry_INT(int initvalue, int id, int eeAddress) :  ConfigEntry(id, eeAddress)
    {
      this->value = initvalue;
    }

void setValue(int value){this->value = value;}
int getValue(){return value;}

void saveValueToEEPROM(){EEPROM.put(eeAddress, value);}
void loadValueFromEEPROM(){EEPROM.get(eeAddress, value);}

  private:
    int value;
};

class ConfigEntry_LONG : public ConfigEntry
{
public:
    // Konstruktor
    ConfigEntry_LONG(long initvalue, int id, int eeAddress) :  ConfigEntry(id, eeAddress)
    {
      this->value = initvalue;
    }

void setValue(long value){this->value = value;}
long getValue(){return value;}

void saveValueToEEPROM(){EEPROM.put(eeAddress, value);}
void loadValueFromEEPROM(){EEPROM.get(eeAddress, value);}

  private:
    long value;
};

class ConfigEntry_BOOL : public ConfigEntry
{
public:
    // Konstruktor
    ConfigEntry_BOOL(bool initvalue, int id, int eeAddress) :  ConfigEntry(id, eeAddress)
    {
      this->value = initvalue;
    }

void setValue(bool value){this->value = value;}
bool getValue(){return value;}

void saveValueToEEPROM(){EEPROM.put(eeAddress, value);}
void loadValueFromEEPROM(){EEPROM.get(eeAddress, value);}

  private:
    bool value;
};

class ConfigEntry_FLOAT : public ConfigEntry
{
public:
    // Konstruktor
    ConfigEntry_FLOAT(float initvalue, int id, int eeAddress) :  ConfigEntry(id, eeAddress)
    {
      this->value = initvalue;
    }

void setValue(float value){this->value = value;}
float getValue(){return value;}

void saveValueToEEPROM(){EEPROM.put(eeAddress, value);}
void loadValueFromEEPROM(){EEPROM.get(eeAddress, value);}

  private:
    float value;
};