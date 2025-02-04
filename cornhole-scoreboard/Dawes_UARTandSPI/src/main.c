//SCL (pin 12 on LCD to PA 5) is SPI clock
//SDI (pin 14 on LCD to PA 7) is SPI signal
//Negated chip select(pin 16 on LCD to PA 15, CS)

//Bit banging
const char* username = "Meredith";

#include "stm32f0xx.h"

void set_char_msg(int, char);
void nano_wait(unsigned int);
void game(void);
void internal_clock();
void check_wiring();
void autotest();


//Configure 8 bit word size w 2 configureation bits
void small_delay(void) {
    nano_wait(50000);
}

//This is where the serial connection is established.
//his is only the serial connection between computer and computer
//From 362 lab 0
void setup_serial(void)
{
    RCC->AHBENR |= 0x00180000;
    GPIOC->MODER  |= 0x02000000;
    GPIOC->AFR[1] |= 0x00020000;
    GPIOD->MODER  |= 0x00000020;
    GPIOD->AFR[0] |= 0x00000200;
    RCC->APB1ENR |= 0x00100000;
    USART5->CR1 &= ~0x00000001;
    USART5->CR1 |= 0x00008000;
    USART5->BRR = 0x340;
    USART5->CR1 |= 0x0000000c;
    USART5->CR1 |= 0x00000001;
}

//UART vs SPI vs I2C
void uart1_setup(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    //Pins 9 10
    //0000 0000 0010 1000 0000 0000 0000 0000
    //0000 0000 0011 1100 0000 0000 0000 0000 (NOT)
    //00280000
    //10 = AF mode
    GPIOA->MODER &= ~0x3c0000;
    GPIOA->MODER |= 0x00280000;
    //0000 0000 0000 0000 0000 1111 1111 0000
    //Both set to AF1: 0001
    // 0001 0001 0000
    GPIOA->AFR[1] &= ~0xff0;
    GPIOA->AFR[1] |= 0x110;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    USART1->CR1 &= ~0x00000001;
    //0000 0000 0000 0000 1000 0000 0000 0000
    //Oversampling? 
    USART1->CR1 |= 0x00008000;
    USART1->BRR = 0x340;
    USART1->CR1 |= 0x0000000c;
    USART1->CR1 |= 0x00000001;


}


//UART setup from rapsberry pi to 
//From 362 lab 6
void init_tim15(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    //48000000/48000 = 1000
    TIM15->PSC = 240 -1;
    TIM15->ARR = 200 - 1;
    TIM15->DIER |= TIM_DIER_UDE;
    TIM15->CR1 |= TIM_CR1_CEN;
}


//From 362 lab 6, with modifications for RFID communication
void init_spi1() {
    //Configure NSS, SCK, MOSI, MISO of SPI1 to PA15, PA5, PA7, PA6
    //Configure SPI register for 10-bit data size
    //enable
    //NSS ->PA15
    //SCK -> PA5
    //MISO -> PA6
    //MOSI -> PA7
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    //1100 0000 0000 0000 1111 1100 0000 0000
    GPIOA->MODER &= ~0xC000fc00;
    //1000 0000 0000 0000 1010 1000 0000 0000
    GPIOA->MODER |= 0x8000a800;
    GPIOA->AFR[0] &= ~0xfff00000;
    GPIOA->AFR[1] &= ~0xf0000000;

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    //clear CR1_SPE bit
    SPI1->CR1 &= ~SPI_CR1_SPE;
    //Set baud rate as low as possible
    SPI1->CR1 |= 0x38;
    //Configure interface for 16-bit word size
    //1010 0000 0000
    SPI1->CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_0;
    //Configure SPI channel to be in slave configuration?
    SPI1->CR1 &= ~SPI_CR1_MSTR;
    //Set SS Output enable bit 
    SPI1->CR2 |= SPI_CR2_SSOE;
    //Set RXDMAEN to enable DMA transfers on recieve buffer occupied
    //Bit 6 RXNEIE: RX buffer not empty interrupt enable
    SPI1->CR2 |= SPI_CR2_RXDMAEN;
    //Enable SPI channel
    SPI1->CR1 |= SPI_CR1_SPE;
    
}
void spi_cmd() {
    //If the receive buffer is not empty, RXNE goes to 1 and so the data is put in the register
    while((SPI1->SR & SPI_SR_RXNE) == 0)
    {}
    //Data is the address of the chip that got scanned, need to figure out how this would work
    //I just want to put the recieved data in the data register. This only takes 16 bits
    //The addresses of the chips are 
    //SPI1->DR = ;

}

//I dont think this is necessary since we will just put the recieved data in the data register
/*void spi_data(unsigned int data) {
    //Calls Spi_cmd w (data | 0x200)
    spi_cmd(data | 0x200);
    
}*/



//===========================================================================
// Configure the proper DMA channel to be triggered by SPI1_RX.
// Set the SPI1 peripheral to trigger a DMA when the receiver is occupied.
//===========================================================================
void spi1_setup_dma(void) {
    //Clock
    //DMA1 channel that Timer 15 uses: Channel 5 
    // 0000
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;

    //EN clck to DMA controller
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    //Set CMAR to address of msg array
    DMA1_Channel3->CMAR = &display;
    //CPAR = addr of GPIOB_ODR register
    DMA1_Channel3->CPAR = (uint32_t) &(SPI1->DR);
    DMA1_Channel3->CNDTR = 34;
    DMA1_Channel3->CCR |= DMA_CCR_DIR;
    //000 0100 0000 0000
    //MSIZE
    DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0;
    DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0;
    DMA1_Channel3->CCR |= DMA_CCR_MINC;
    DMA1_Channel3->CCR |= DMA_CCR_CIRC;
    SPI1->CR2 |= 0x00000002;
    
    
}

//===========================================================================
// Enable the DMA channel triggered by SPI1_TX.
//===========================================================================
void spi1_enable_dma(void) {
    DMA1_Channel3->CCR |= DMA_CCR_EN;
    
}



//===========================================================================
// Main function
//===========================================================================

int main(void) {
    internal_clock();

    setup_serial();
    uart1_setup();
    while(1){
        //Send message on this keyboard
        if((USART5->ISR & USART_ISR_RXNE))
            //Put message into uart 1 transfer portal
            USART1->TDR = USART5->RDR;
            //USART5->TDR = USART5->RDR;
        //Receive message? I think this part will work if I can get the ports open on the pi
        if((USART1->ISR & USART_ISR_RXNE))
            //print it out here? Idk
            USART5->TDR = USART1->RDR;
            //USART1->TDR = USART1->RDR;

    }

    init_spi1();
  
}
