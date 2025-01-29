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
    int id; // stores the unique id from each Configitem, beginning at 0

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

class ConfigEntry_GLASS : public ConfigEntry
{
public:
    // Konstruktor
    ConfigEntry_GLASS(int fillweight, int emptyweight, int id, int eeAddress) :  ConfigEntry(id, eeAddress)
    {
      this->fillweight = fillweight;
      this->emptyweight = emptyweight;
    }

    //Standardkonstruktor
    ConfigEntry_GLASS() : ConfigEntry(0, 0), fillweight(0), emptyweight(0) {}

  void setFillweight(int fillweight){this->fillweight = fillweight;}
  void setEmptyweight(int emptyweight){this->emptyweight = emptyweight;}
  int getFillweight(){return fillweight;}
  int getEmptyweight(){return emptyweight;}

  void saveFillweightToEEPROM(){EEPROM.put(eeAddress, fillweight);}
  void loadFillweightFromEEPROM(){EEPROM.get(eeAddress, fillweight);}

  void saveEmptyweightToEEPROM(){EEPROM.put(eeAddress + sizeof(fillweight), emptyweight);}
  void loadEmptyweightFromEEPROM(){EEPROM.get(eeAddress + sizeof(fillweight), emptyweight);}

  void loadValuesFromEEPROM(){
    loadFillweightFromEEPROM();
    loadEmptyweightFromEEPROM();
  }

  private:
    int fillweight;
    int emptyweight;
};