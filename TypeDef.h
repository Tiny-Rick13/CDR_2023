#include <stdint.h>

#define     __IO    volatile             /*!< Defines 'read / write' permissions */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      
  HardFault_IRQn                = -13,      
  SVCall_IRQn                   = -5,       
  PendSV_IRQn                   = -2,       
  SysTick_IRQn                  = -1,   
} IRQn_Type;

typedef struct UART_InitTypeDef
{
  uint32_t BaudRate;                /*!< This member configures the UART communication baud rate.
                                         The baud rate register is computed using the following formula:
                                         LPUART:
                                         =======
                                         Baud Rate Register = ((256 * lpuart_ker_ckpres) / ((huart->Init.BaudRate)))
                                         where lpuart_ker_ck_pres is the UART input clock divided by a prescaler
                                         UART:
                                         =====
                                         - If oversampling is 16 or in LIN mode,
                                            Baud Rate Register = ((uart_ker_ckpres) / ((huart->Init.BaudRate)))
                                         - If oversampling is 8,
                                            Baud Rate Register[15:4] = ((2 * uart_ker_ckpres) /
                                            ((huart->Init.BaudRate)))[15:4]
                                            Baud Rate Register[3] =  0
                                            Baud Rate Register[2:0] =  (((2 * uart_ker_ckpres) /
                                            ((huart->Init.BaudRate)))[3:0]) >> 1
                                         where uart_ker_ck_pres is the UART input clock divided by a prescaler */

  uint32_t WordLength;              /*!< Specifies the number of data bits transmitted or received in a frame.
                                         This parameter can be a value of @ref UARTEx_Word_Length. */

  uint32_t StopBits;                /*!< Specifies the number of stop bits transmitted.
                                         This parameter can be a value of @ref UART_Stop_Bits. */

  uint32_t Parity;                  /*!< Specifies the parity mode.
                                         This parameter can be a value of @ref UART_Parity
                                         @note When parity is enabled, the computed parity is inserted
                                               at the MSB position of the transmitted data (9th bit when
                                               the word length is set to 9 data bits; 8th bit when the
                                               word length is set to 8 data bits). */

  uint32_t Mode;                    /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                         This parameter can be a value of @ref UART_Mode. */

  uint32_t HwFlowCtl;               /*!< Specifies whether the hardware flow control mode is enabled
                                         or disabled.
                                         This parameter can be a value of @ref UART_Hardware_Flow_Control. */

  uint32_t OverSampling;            /*!< Specifies whether the Over sampling 8 is enabled or disabled,
                                         to achieve higher speed (up to f_PCLK/8).
                                         This parameter can be a value of @ref UART_Over_Sampling. */

  uint32_t OneBitSampling;          /*!< Specifies whether a single sample or three samples' majority vote is selected.
                                         Selecting the single sample method increases the receiver tolerance to clock
                                         deviations. This parameter can be a value of @ref UART_OneBit_Sampling. */

  uint32_t ClockPrescaler;          /*!< Specifies the prescaler value used to divide the UART clock source.
                                         This parameter can be a value of @ref UART_ClockPrescaler. */

} UART_InitTypeDef;

/**
  * @brief  UART Advanced Features initialization structure definition
  */

typedef struct UART_AdvFeatureInitTypeDef
{
  uint32_t AdvFeatureInit;        /*!< Specifies which advanced UART features is initialized. Several
                                       Advanced Features may be initialized at the same time .
                                       This parameter can be a value of
                                       @ref UART_Advanced_Features_Initialization_Type. */

  uint32_t TxPinLevelInvert;      /*!< Specifies whether the TX pin active level is inverted.
                                       This parameter can be a value of @ref UART_Tx_Inv. */

  uint32_t RxPinLevelInvert;      /*!< Specifies whether the RX pin active level is inverted.
                                       This parameter can be a value of @ref UART_Rx_Inv. */

  uint32_t DataInvert;            /*!< Specifies whether data are inverted (positive/direct logic
                                       vs negative/inverted logic).
                                       This parameter can be a value of @ref UART_Data_Inv. */

  uint32_t Swap;                  /*!< Specifies whether TX and RX pins are swapped.
                                       This parameter can be a value of @ref UART_Rx_Tx_Swap. */

  uint32_t OverrunDisable;        /*!< Specifies whether the reception overrun detection is disabled.
                                       This parameter can be a value of @ref UART_Overrun_Disable. */

  uint32_t DMADisableonRxError;   /*!< Specifies whether the DMA is disabled in case of reception error.
                                       This parameter can be a value of @ref UART_DMA_Disable_on_Rx_Error. */

  uint32_t AutoBaudRateEnable;    /*!< Specifies whether auto Baud rate detection is enabled.
                                       This parameter can be a value of @ref UART_AutoBaudRate_Enable. */

  uint32_t AutoBaudRateMode;      /*!< If auto Baud rate detection is enabled, specifies how the rate
                                       detection is carried out.
                                       This parameter can be a value of @ref UART_AutoBaud_Rate_Mode. */

  uint32_t MSBFirst;              /*!< Specifies whether MSB is sent first on UART line.
                                       This parameter can be a value of @ref UART_MSB_First. */
} UART_AdvFeatureInitTypeDef;

/**
  * @brief HAL UART State definition
  * @note  HAL UART State value is a combination of 2 different substates:
  *        gState and RxState (see @ref UART_State_Definition).
  *        - gState contains UART state information related to global Handle management
  *          and also information related to Tx operations.
  *          gState value coding follow below described bitmap :
  *          b7-b6  Error information
  *             00 : No Error
  *             01 : (Not Used)
  *             10 : Timeout
  *             11 : Error
  *          b5     Peripheral initialization status
  *             0  : Reset (Peripheral not initialized)
  *             1  : Init done (Peripheral initialized. HAL UART Init function already called)
  *          b4-b3  (not used)
  *             xx : Should be set to 00
  *          b2     Intrinsic process state
  *             0  : Ready
  *             1  : Busy (Peripheral busy with some configuration or internal operations)
  *          b1     (not used)
  *             x  : Should be set to 0
  *          b0     Tx state
  *             0  : Ready (no Tx operation ongoing)
  *             1  : Busy (Tx operation ongoing)
  *        - RxState contains information related to Rx operations.
  *          RxState value coding follow below described bitmap :
  *          b7-b6  (not used)
  *             xx : Should be set to 00
  *          b5     Peripheral initialization status
  *             0  : Reset (Peripheral not initialized)
  *             1  : Init done (Peripheral initialized)
  *          b4-b2  (not used)
  *            xxx : Should be set to 000
  *          b1     Rx state
  *             0  : Ready (no Rx operation ongoing)
  *             1  : Busy (Rx operation ongoing)
  *          b0     (not used)
  *             x  : Should be set to 0.
  */
typedef uint32_t HAL_UART_StateTypeDef;

/**
  * @brief UART clock sources definition
  */
typedef enum UART_ClockSourceTypeDef
{
  UART_CLOCKSOURCE_PCLK1      = 0x00U,    /*!< PCLK1 clock source  */
  UART_CLOCKSOURCE_PCLK2      = 0x01U,    /*!< PCLK2 clock source  */
  UART_CLOCKSOURCE_HSI        = 0x02U,    /*!< HSI clock source    */
  UART_CLOCKSOURCE_SYSCLK     = 0x04U,    /*!< SYSCLK clock source */
  UART_CLOCKSOURCE_LSE        = 0x08U,    /*!< LSE clock source       */
  UART_CLOCKSOURCE_UNDEFINED  = 0x10U     /*!< Undefined clock source */
} UART_ClockSourceTypeDef;

/**
  * @brief HAL UART Reception type definition
  * @note  HAL UART Reception type value aims to identify which type of Reception is ongoing.
  *        It is expected to admit following values :
  *           HAL_UART_RECEPTION_STANDARD         = 0x00U,
  *           HAL_UART_RECEPTION_TOIDLE           = 0x01U,
  *           HAL_UART_RECEPTION_TORTO            = 0x02U,
  *           HAL_UART_RECEPTION_TOCHARMATCH      = 0x03U,
  */
typedef uint32_t HAL_UART_RxTypeTypeDef;

/**
  * @brief  UART handle Structure definition
  */

/**
  * @brief  HAL Status structures definition
  */
typedef enum HAL_StatusTypeDef
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/**
  * @brief  HAL Lock structures definition
  */
typedef enum HAL_LockTypeDef
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

typedef struct UART_WakeUpTypeDef
{
  uint32_t WakeUpEvent;        /*!< Specifies which event will activate the Wakeup from Stop mode flag (WUF).
                                    This parameter can be a value of @ref UART_WakeUp_from_Stop_Selection.
                                    If set to UART_WAKEUP_ON_ADDRESS, the two other fields below must
                                    be filled up. */

  uint16_t AddressLength;      /*!< Specifies whether the address is 4 or 7-bit long.
                                    This parameter can be a value of @ref UARTEx_WakeUp_Address_Length.  */

  uint8_t Address;             /*!< UART/USART node address (7-bit long max). */
} UART_WakeUpTypeDef;

typedef struct USART_TypeDef
{
  __IO uint32_t CR1;         /*!< USART Control register 1,                 Address offset: 0x00  */
  __IO uint32_t CR2;         /*!< USART Control register 2,                 Address offset: 0x04  */
  __IO uint32_t CR3;         /*!< USART Control register 3,                 Address offset: 0x08  */
  __IO uint32_t BRR;         /*!< USART Baud rate register,                 Address offset: 0x0C  */
  __IO uint32_t GTPR;        /*!< USART Guard time and prescaler register,  Address offset: 0x10  */
  __IO uint32_t RTOR;        /*!< USART Receiver Timeout register,          Address offset: 0x14  */
  __IO uint32_t RQR;         /*!< USART Request register,                   Address offset: 0x18  */
  __IO uint32_t ISR;         /*!< USART Interrupt and status register,      Address offset: 0x1C  */
  __IO uint32_t ICR;         /*!< USART Interrupt flag Clear register,      Address offset: 0x20  */
  __IO uint32_t RDR;         /*!< USART Receive Data register,              Address offset: 0x24  */
  __IO uint32_t TDR;         /*!< USART Transmit Data register,             Address offset: 0x28  */
  __IO uint32_t PRESC;       /*!< USART Prescaler register,                 Address offset: 0x2C  */
} USART_TypeDef;

typedef struct DMA_Channel_TypeDef
{
  __IO uint32_t CCR;         /*!< DMA channel x configuration register        */
  __IO uint32_t CNDTR;       /*!< DMA channel x number of data register       */
  __IO uint32_t CPAR;        /*!< DMA channel x peripheral address register   */
  __IO uint32_t CMAR;        /*!< DMA channel x memory address register       */
} DMA_Channel_TypeDef;

typedef struct DMA_InitTypeDef
{
  uint32_t Request;                   /*!< Specifies the request selected for the specified channel.
                                           This parameter can be a value of @ref DMA_request */

  uint32_t Direction;                 /*!< Specifies if the data will be transferred from memory to peripheral,
                                           from memory to memory or from peripheral to memory.
                                           This parameter can be a value of @ref DMA_Data_transfer_direction */

  uint32_t PeriphInc;                 /*!< Specifies whether the Peripheral address register should be incremented or not.
                                           This parameter can be a value of @ref DMA_Peripheral_incremented_mode */

  uint32_t MemInc;                    /*!< Specifies whether the memory address register should be incremented or not.
                                           This parameter can be a value of @ref DMA_Memory_incremented_mode */

  uint32_t PeriphDataAlignment;       /*!< Specifies the Peripheral data width.
                                           This parameter can be a value of @ref DMA_Peripheral_data_size */

  uint32_t MemDataAlignment;          /*!< Specifies the Memory data width.
                                           This parameter can be a value of @ref DMA_Memory_data_size */

  uint32_t Mode;                      /*!< Specifies the operation mode of the DMAy Channelx.
                                           This parameter can be a value of @ref DMA_mode
                                           @note The circular buffer mode cannot be used if the memory-to-memory
                                                 data transfer is configured on the selected Channel */

  uint32_t Priority;                  /*!< Specifies the software priority for the DMAy Channelx.
                                           This parameter can be a value of @ref DMA_Priority_level */
} DMA_InitTypeDef;

typedef enum HAL_DMA_StateTypeDef
{
  HAL_DMA_STATE_RESET             = 0x00U,  /*!< DMA not yet initialized or disabled    */
  HAL_DMA_STATE_READY             = 0x01U,  /*!< DMA initialized and ready for use      */
  HAL_DMA_STATE_BUSY              = 0x02U,  /*!< DMA process is ongoing                 */
  HAL_DMA_STATE_TIMEOUT           = 0x03U,  /*!< DMA timeout state                     */
} HAL_DMA_StateTypeDef;

typedef struct DMA_TypeDef
{
  __IO uint32_t ISR;         /*!< DMA interrupt status register,                 Address offset: 0x00 */
  __IO uint32_t IFCR;        /*!< DMA interrupt flag clear register,             Address offset: 0x04 */
} DMA_TypeDef;

typedef struct DMAMUX_Channel_TypeDef
{
  __IO uint32_t   CCR;       /*!< DMA Multiplexer Channel x Control Register    Address offset: 0x0004 * (channel x) */
}DMAMUX_Channel_TypeDef;

typedef struct DMAMUX_ChannelStatus_TypeDef
{
  __IO uint32_t   CSR;      /*!< DMA Channel Status Register                    Address offset: 0x0080   */
  __IO uint32_t   CFR;      /*!< DMA Channel Clear Flag Register                Address offset: 0x0084   */
}DMAMUX_ChannelStatus_TypeDef;

typedef struct DMAMUX_RequestGen_TypeDef
{
  __IO uint32_t   RGCR;        /*!< DMA Request Generator x Control Register     Address offset: 0x0100 + 0x0004 * (Req Gen x) */
}DMAMUX_RequestGen_TypeDef;

typedef struct DMAMUX_RequestGenStatus_TypeDef
{
  __IO uint32_t   RGSR;        /*!< DMA Request Generator Status Register        Address offset: 0x0140   */
  __IO uint32_t   RGCFR;        /*!< DMA Request Generator Clear Flag Register    Address offset: 0x0144   */
}DMAMUX_RequestGenStatus_TypeDef;

typedef struct __DMA_HandleTypeDefU
{
  DMA_Channel_TypeDef    *Instance;                                                  /*!< Register base address                */

  DMA_InitTypeDef       Init;                                                        /*!< DMA communication parameters         */

  HAL_LockTypeDef       Lock;                                                        /*!< DMA locking object                   */

  __IO HAL_DMA_StateTypeDef  State;                                                  /*!< DMA transfer state                   */

  void                  *Parent;                                                     /*!< Parent object state                  */

  void (* XferCpltCallback)(struct __DMA_HandleTypeDef *hdma);                       /*!< DMA transfer complete callback       */

  void (* XferHalfCpltCallback)(struct __DMA_HandleTypeDef *hdma);                   /*!< DMA Half transfer complete callback  */

  void (* XferErrorCallback)(struct __DMA_HandleTypeDef *hdma);                      /*!< DMA transfer error callback          */

  void (* XferAbortCallback)(struct __DMA_HandleTypeDef *hdma);                      /*!< DMA transfer abort callback          */

  __IO uint32_t          ErrorCode;                                                  /*!< DMA Error code                       */

  DMA_TypeDef            *DmaBaseAddress;                                            /*!< DMA Channel Base Address             */

  uint32_t               ChannelIndex;                                               /*!< DMA Channel Index                    */

  DMAMUX_Channel_TypeDef           *DMAmuxChannel;                                   /*!< Register base address                */

  DMAMUX_ChannelStatus_TypeDef     *DMAmuxChannelStatus;                             /*!< DMAMUX Channels Status Base Address  */

  uint32_t                         DMAmuxChannelStatusMask;                          /*!< DMAMUX Channel Status Mask           */

  DMAMUX_RequestGen_TypeDef        *DMAmuxRequestGen;                                /*!< DMAMUX request generator Base Address */

  DMAMUX_RequestGenStatus_TypeDef  *DMAmuxRequestGenStatus;                          /*!< DMAMUX request generator Address     */

  uint32_t                         DMAmuxRequestGenStatusMask;                       /*!< DMAMUX request generator Status mask */

} DMA_HandleTypeDef;


typedef struct __UART_HandleTypeDef
{
  USART_TypeDef            *Instance;                /*!< UART registers base address        */

  UART_InitTypeDef         Init;                     /*!< UART communication parameters      */

  UART_AdvFeatureInitTypeDef AdvancedInit;           /*!< UART Advanced Features initialization parameters */

  const uint8_t            *pTxBuffPtr;              /*!< Pointer to UART Tx transfer Buffer */

  uint16_t                 TxXferSize;               /*!< UART Tx Transfer size              */

  __IO uint16_t            TxXferCount;              /*!< UART Tx Transfer Counter           */

  uint8_t                  *pRxBuffPtr;              /*!< Pointer to UART Rx transfer Buffer */

  uint16_t                 RxXferSize;               /*!< UART Rx Transfer size              */

  __IO uint16_t            RxXferCount;              /*!< UART Rx Transfer Counter           */

  uint16_t                 Mask;                     /*!< UART Rx RDR register mask          */

  uint32_t                 FifoMode;                 /*!< Specifies if the FIFO mode is being used.
                                                          This parameter can be a value of @ref UARTEx_FIFO_mode. */

  uint16_t                 NbRxDataToProcess;        /*!< Number of data to process during RX ISR execution */

  uint16_t                 NbTxDataToProcess;        /*!< Number of data to process during TX ISR execution */

  __IO HAL_UART_RxTypeTypeDef ReceptionType;         /*!< Type of ongoing reception          */

  void (*RxISR)(struct __UART_HandleTypeDef *huart); /*!< Function pointer on Rx IRQ handler */

  void (*TxISR)(struct __UART_HandleTypeDef *huart); /*!< Function pointer on Tx IRQ handler */

  DMA_HandleTypeDef        *hdmatx;                  /*!< UART Tx DMA Handle parameters      */

  DMA_HandleTypeDef        *hdmarx;                  /*!< UART Rx DMA Handle parameters      */

  HAL_LockTypeDef           Lock;                    /*!< Locking object                     */

  __IO HAL_UART_StateTypeDef    gState;              /*!< UART state information related to global Handle management
                                                          and also related to Tx operations. This parameter
                                                          can be a value of @ref HAL_UART_StateTypeDef */

  __IO HAL_UART_StateTypeDef    RxState;             /*!< UART state information related to Rx operations. This
                                                          parameter can be a value of @ref HAL_UART_StateTypeDef */

  __IO uint32_t                 ErrorCode;           /*!< UART Error code                    */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  void (* TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Tx Half Complete Callback        */
  void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Tx Complete Callback             */
  void (* RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Rx Half Complete Callback        */
  void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Complete Callback             */
  void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART Error Callback                   */
  void (* AbortCpltCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Abort Complete Callback          */
  void (* AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart); /*!< UART Abort Transmit Complete Callback */
  void (* AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);  /*!< UART Abort Receive Complete Callback  */
  void (* WakeupCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Wakeup Callback                  */
  void (* RxFifoFullCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Rx Fifo Full Callback            */
  void (* TxFifoEmptyCallback)(struct __UART_HandleTypeDef *huart);       /*!< UART Tx Fifo Empty Callback           */
  void (* RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< UART Reception Event Callback     */

  void (* MspInitCallback)(struct __UART_HandleTypeDef *huart);           /*!< UART Msp Init callback                */
  void (* MspDeInitCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Msp DeInit callback              */
#endif  /* USE_HAL_UART_REGISTER_CALLBACKS */

} UART_HandleTypeDef;



