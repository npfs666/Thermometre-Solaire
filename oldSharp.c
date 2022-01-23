/*void SharpStaticMode() {
    
    uint8_t i;
    
    SHARP_CLK = 0;
    SHARP_CS = 1;   // Selection de la puce + tempo associée
    __delay_us(6);
    
    send(0);    // M0
    send(switchVCOM());    // M1
    send(0);    // M2
    
    for( i = 0; i < 14; i++)
        send(0);
    
    __delay_us(2);
    SHARP_CS = 0;
}*/

/*void SharpClearDisplay() {
    
    uint16_t i,j;
    
    // RAZ (1) du buffer
    for(i = 0; i < 96; i++ ) {
        
        for( j=0; j < 12; j++) {
            buffer[j][i] = 0xFF;
        }
    } 
    
    // Envoi de la cde clear
    SHARP_CLK = 0;
    SHARP_CS = 1;   // Selection de la puce + tempo associée
    __delay_us(6);
    
    send(0);    // M0
    send(switchVCOM());    // M1
    send(1);    // M2
    
    for( i = 0; i < 14; i++)
        send(0);
    
    __delay_us(2);
    SHARP_CS = 0;
}*/

/*void send(uint8_t data) {
    
    SHARP_DI = data;
    //__delay_us(1);
    SHARP_CLK = 1;
    //__delay_us(1);
    SHARP_CLK = 0;
}*/

/*void SharpRefresh() {
    
    uint8_t i,j,k;
    
    
    SHARP_CLK = 0;
    SHARP_CS = 1;   // Selection de la puce + tempo associée
    __delay_us(6);

    for( k = 0; k < 96; k++) {

        // Entete
        send(1); // M0
        send(switchVCOM()); // M1
        send(0); // M2
        for ( i = 0; i < 5; i++ ) {
            send(0);
        }

        // Adresse (1-96))
        for ( i = 0; i < 8; i++ ) {

            if ( k & (set[i]) ) {
                send(1);
            } else {
                send(0);
            }
        }

        // Data
        for ( i = 0; i < 12; i++ ) {

            for ( j = 0; j < 8; j++ ) {

                if ( (buffer[i][k - 1]) & set[j] ) {
                    send(1);
                } else {
                    send(0);
                }
            }
        }
    }

    // Data tranfert period
    for ( i = 0; i < 16; i++ )
        send(0);

    __delay_us(2);
    SHARP_CS = 0;
    
}*/
