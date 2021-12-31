// IO pins
const int RELAY_PIN = 2;
const int BUTTON_PIN = 4;
const int BUZZER_PIN = 7;
const int CONTACTS_PIN = 8;

// defined values
const unsigned long UV_RUN_TIME = 30000;

const int BUTTON_DEBOUNCE_DELAY = 60;
const int CONTACTS_DISCONNECTED_DELAY = 200;

const int BUZZER_CONNECT_DURATION = 20;
const int BUZZER_DISCONNECT_DURATION = 400;
const int BUZZER_BEEPBEEP_DURATION = 70;
const int BUZZER_BEEPBEEP_DELAY = 120;

// for button debounce
int buttonLastSteadyState = HIGH;
int buttonlastFlickerableState  = HIGH;
int buttonCurrentState;
unsigned long buttonLastDebounceTime = 0;

// runtime variables
bool runUV = false;
unsigned long uvStartTime;

// contacts variables
bool contactsConnected = false;
unsigned long contactsLastConnected = 0;


void setup(){
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(CONTACTS_PIN, INPUT_PULLUP);
    // pinMode(LED_BUILTIN, OUTPUT);
}


void loop(){
    // on event of button press
    if (buttonPressed()){
        if (runUV == false){
            buzzerBeep(BUZZER_CONNECT_DURATION);
            runUV = true;
            uvStartTime = millis();
        }
    }

    // ensure contacts are connected
    if (runUV){
        checkContacts();
        if (contactsConnected == false){
            if ((millis() - contactsLastConnected) >= CONTACTS_DISCONNECTED_DELAY){
                digitalWrite(RELAY_PIN, LOW);
                buzzerBeepBeepBeep(3);
                runUV = false;
            }
        }
    }

    // running logic
    if (runUV){
        if ((millis() - uvStartTime) < UV_RUN_TIME){
            // digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(RELAY_PIN, HIGH);
        } else {
            // digitalWrite(LED_BUILTIN, LOW);
            digitalWrite(RELAY_PIN, LOW);
            buzzerBeep(BUZZER_DISCONNECT_DURATION);
            runUV = false;
        }
    } else {
        // digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RELAY_PIN, LOW);
    }
}


bool buttonPressed(){
    buttonCurrentState = digitalRead(BUTTON_PIN);

    // set change in state as flickerable
    if (buttonCurrentState != buttonlastFlickerableState){
        buttonLastDebounceTime = millis();

        buttonlastFlickerableState = buttonCurrentState;
    }

    // after delay if flickerable state doesn't change
    if ((millis() - buttonLastDebounceTime) > BUTTON_DEBOUNCE_DELAY){
        if (buttonLastSteadyState == HIGH && buttonCurrentState == LOW){
            return true;
        } else if (buttonLastSteadyState == LOW && buttonCurrentState == HIGH){
            return false;
        }

        buttonLastSteadyState = buttonCurrentState;
    } else {
        return false;
    }
}


void buzzerBeep(int duration){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}


void buzzerBeepBeepBeep(int repeats){
    for (int i=0; i<repeats; i++){
        buzzerBeep(BUZZER_BEEPBEEP_DURATION);
        delay(BUZZER_BEEPBEEP_DELAY);
    }
}


void checkContacts(){
    if (digitalRead(CONTACTS_PIN) == LOW){
        contactsConnected = true;
        contactsLastConnected = millis();
    } else {
        contactsConnected = false;
    }
}