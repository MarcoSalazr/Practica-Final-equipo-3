#include "IRremote.h"

// Definición de pines
#define relay 7         // Pin al que está conectado el relé
#define infrared 11     // Pin analógico para el sensor infrarrojo
#define touch A2        // Pin digital para el sensor táctil
#define sound A3        // Pin analógico para el sensor de sonido

int VALOR;               // Variable para almacenar el valor del sensor de sonido

void setup() {
  Serial.begin(9600);    // Inicialización de la comunicación serial a 9600 bps
  IrReceiver.begin(infrared); //Se inicializa la comunicacion con el sensor infrarojo
  pinMode(relay, OUTPUT); // Configurar el pin del relé como salida
  pinMode(touch, INPUT);  // Configurar el pin del sensor táctil como entrada
  pinMode(sound, INPUT);  // Configurar el pin del sensor de sonido como entrada
  digitalWrite(relay, LOW); // Inicialmente, apagar el relé
}

//Funcion encargada de cambiar el estado de relay, en este caso encender o apagar el foco
void changeStatusRelay() {
  digitalWrite(relay, !digitalRead(relay)); //Esta linea invierte el valor del relay, de HIGH a LOW y viceversa
  delay(500); //Delay para evitar repeticion instantanea
}

//Funcion encargada de revisar el estatus del sensor infrarojo, en caso de detectar una señal
// y que esta coincida con el valor del boton de encendido se enciende el foco
void irCheck() {
  // Verificar si hay una señal infrarroja recibida
  if (IrReceiver.decode()) {
    // Imprime los datos de la señal cuando registra una lectura
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    // Compara si es correcto
    if (IrReceiver.decodedIRData.decodedRawData == 0xBA45FF00) {
    //Cambia estatus del relay
      changeStatusRelay();
    }
    //Restablece el sensor para que nuevamente reciba señales
    IrReceiver.resume();
  }
}

//Revisa el sensor a ver si hay un cambio en la lectura, y en caso de que la haya enciende el foco
void soundCheck() {
  // Leer el valor del sensor de sonido
  VALOR = analogRead(sound);

  // Activar el relé si el valor del sensor de sonido es mayor que 34
  if (VALOR > 34) {
    Serial.println(VALOR);
    changeStatusRelay();
  }
}

//Esta funcion revisa si hay un toque en el sensor y en caso que suceda enciende el foco
void touchCheck() {
  // Verificar si el sensor táctil está siendo presionado
  if (digitalRead(touch)) {
    changeStatusRelay();
  }
}

//Se encarga de ciclar el codigo, haciendo las lecturas de los sensores
void loop() {
  irCheck();
  soundCheck();
  touchCheck();
}
