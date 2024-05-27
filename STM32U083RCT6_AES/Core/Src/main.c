/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "aes.h"
#include "dma.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
  AES_ALGO_ECB,
  AES_ALGO_CBC,
} AES_Algorithm_t;

typedef enum {
  AES_ENCRYPTO_MODE,
  AES_DECRYPTO_MODE,
} AES_Crypt_Mode_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AES_BLOCK_SIZE  16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BLSWAP32(val) \
    ((val) = ((uint32_t)((((uint32_t)(val) & (uint32_t)0x000000ffU) << 24) | \
          (((uint32_t)(val) & (uint32_t)0x0000ff00U) <<  8) | \
          (((uint32_t)(val) & (uint32_t)0x00ff0000U) >>  8) | \
          (((uint32_t)(val) & (uint32_t)0xff000000U) >> 24))))

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint32_t task1 = 0;

uint8_t AES_key1[33] = "12345678901234561234567890123456";
uint8_t aesiv1[17] = "1234567890123456";

uint8_t AES_key[32] = {0};
uint8_t aesiv[16] = {0};

char Text1[] = " \
-----BEGIN PRIVATE KEY-----                                      \
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 \
7IbiS/98RTtjVN4NIQJ9Jc8hUhkW                                     \
-----END PRIVATE KEY-----                                        \
";

char Text2[] = "1234qwer5678asdf1234qwer5678asdf";

char *Text = Text1;

uint8_t plain[4096 + 16] = {0};
uint8_t cipher[4096 + 16] = {0};
uint8_t tmp_buffer[4096 + 16] = {0};

int i = 0;
uint32_t start, end, interval;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void show_hex(unsigned char *p, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%02x ", p[i]);
        if ((i+1)%16 == 0) printf("\n");
    }
    printf("\n");
}

uint8_t task_scheduling(uint32_t *task, uint32_t time)
{
  uint32_t sys_ms_timer = HAL_GetTick();
  if ((sys_ms_timer > *task) && ((sys_ms_timer - *task) > time))
  {
    *task = sys_ms_timer;
    return 1;
  }
  else if (*task > sys_ms_timer)
  {
    if (((0xffffffff - *task) + sys_ms_timer) > time)
    {
      *task = sys_ms_timer;
      return 1;
    }
  }
  return 0;
}

static size_t pkcs7Padding(unsigned char *p, size_t len, uint8_t block_size)
{
    uint8_t i = 0;
    uint8_t padding = 0;
    padding = block_size - len%block_size;

    for (i = 0; i < padding; i++) {
        p[len + i] = padding;
    }

    return (len + padding);
}

static size_t pkcs7UnPadding(unsigned char *p, size_t len, uint8_t block_size)
{
    uint8_t unPadding  = 0;
    unPadding  = p[len - 1];
    int i = 0;

    if (unPadding > 16) {
        return -1;
    }

    for (i = 0; i < unPadding; i++) {
        if (p[len - 1 - i] != unPadding)
            return -1;
    }

    return (len - unPadding);
}

int aes_set_config(uint8_t *key, int key_len, AES_Algorithm_t mode, uint8_t *iv)
{
  int ret = -1;
  CRYP_ConfigTypeDef cryp_conf = {0};
  do {
    if (key_len != 32 || key == NULL) {
      printf("error: The key length only supports 32\n");
      break;
    }

    if (mode == CRYP_AES_CBC && iv == NULL) {
      printf("error: iv param is null\n");
      break;
    }

    for (int i = 0; i < key_len >> 2; i ++)
        BLSWAP32(((uint32_t*)key)[i]);

    if (HAL_CRYP_GetConfig(&hcryp, &cryp_conf) != HAL_OK) {
      printf("error: HAL_CRYP_GetConfig fail\n");
      break;
    }
    cryp_conf.Algorithm = (mode == AES_ALGO_ECB) ? CRYP_AES_ECB : CRYP_AES_CBC;
    cryp_conf.pKey = (uint32_t *)key;

    if (mode == AES_ALGO_CBC) {
      for (int i = 0; i < 4; i ++)
          BLSWAP32(((uint32_t*)iv)[i]);
      cryp_conf.pInitVect = (uint32_t *)iv;
    }

    if (HAL_CRYP_SetConfig(&hcryp, &cryp_conf) != HAL_OK) {
      printf("error: HAL_CRYP_SetConfig fail\n");
      break;
    }
    ret = 0;
  } while (0);
  return ret;
}

#define CRYPT_NUM_TIMES 10   // 设置加解密测试轮数

int aes_crypt(AES_Crypt_Mode_t mode, unsigned char *in_buffer, int inlen, unsigned char *out_buffer)
{
  int ret = -1;
  int plain_len = 0;

  memmove(tmp_buffer, in_buffer, inlen);

  do
  {

    start = HAL_GetTick();
    for (i = 0; i < CRYPT_NUM_TIMES; i++)
    {
      memmove(in_buffer, tmp_buffer, inlen);

      if (mode == AES_ENCRYPTO_MODE) {
        plain_len = pkcs7Padding((uint8_t *)in_buffer, inlen, AES_BLOCK_SIZE);
        for (int i = 0; i < plain_len >> 2; i++)
          BLSWAP32(((uint32_t *)in_buffer)[i]);

        if (HAL_CRYP_Encrypt(&hcryp, (uint32_t *)in_buffer, plain_len >> 2, (uint32_t *)out_buffer, 0xff) != HAL_OK)
        {
          printf("error: HAL_CRYP_Encrypt fail\n");
          break;
        }

        for (int i = 0; i < plain_len >> 2; i++)
          BLSWAP32(((uint32_t *)out_buffer)[i]);
      } else {
        for (int i = 0; i < inlen >> 2; i++)
          BLSWAP32(((uint32_t *)in_buffer)[i]);
        if (HAL_CRYP_Decrypt(&hcryp, (uint32_t *)in_buffer, inlen >> 2, (uint32_t *)out_buffer, 0xff) != HAL_OK)
        {
          printf("error: HAL_CRYP_Decrypt fail\n");
          break;
        }

        for (int i = 0; i < inlen >> 2; i++)
          BLSWAP32(((uint32_t *)out_buffer)[i]);

        plain_len = pkcs7UnPadding((uint8_t*)out_buffer, inlen, AES_BLOCK_SIZE);
      }

    }
    end = HAL_GetTick();
    if (i != CRYPT_NUM_TIMES) {
      printf("[%d] error: CRYPT_NUM_TIMES i:%d fail\n", __LINE__, i);
      break;
    }

    interval = end - start;
    printf("%s %d times: %ld\n", mode == 0 ? "encrypto" : "decrypto", CRYPT_NUM_TIMES, interval);

    ret = plain_len;
  } while (0);

  return ret;
}

int aes_ecb_test(void)
{
  int ret = -1;
  int text_len = strlen(Text);
  int length = 0;

  do {
    memmove(AES_key, AES_key1, 32);
    aes_set_config(AES_key, 32, AES_ALGO_ECB, NULL);

    printf("==== aes_ecb_test 1 ==== \n");
    memset(plain, 0, sizeof(plain));
    memmove(plain, Text, text_len);

    length = aes_crypt(AES_ENCRYPTO_MODE, plain, text_len, cipher);
    if (length < 0) {
      printf("[%d] error: aes_crypt fail\n", __LINE__);
      break;
    }

    printf("encrypto len: %d\n", length);
    // show_hex(cipher, 64);

    memset(plain, 0, sizeof(plain));

    length = aes_crypt(AES_DECRYPTO_MODE, cipher, length, plain);
    if (length < 0) {
      printf("[%d] error: aes_crypt fail\n", __LINE__);
      break;
    }

    printf("decrypto len: %d\n", length);
    // printf("%.*s\n", 64, plain);

    if (text_len == length && memcmp(plain, Text, length) == 0) {
      printf("\naes_ecb_test success\n");
    } else {
      printf("\naes_ecb_test fail\n");
    }

    ret = 0;
  } while (0);

  return ret;
}

int aes_cbc_test(void)
{
  int ret = -1;
  int text_len = strlen(Text);
  uint16_t length = 0;

  do {
    memmove(AES_key, AES_key1, 32);
    memmove(aesiv, aesiv1, 16);
    aes_set_config(AES_key, 32, AES_ALGO_CBC, aesiv);

    printf("==== aes_cbc_test 1 ==== \n");
    memset(plain, 0, sizeof(plain));
    memmove(plain, Text, text_len);

    length = aes_crypt(AES_ENCRYPTO_MODE, plain, text_len, cipher);
    if (length < 0) {
      printf("[%d] error: aes_crypt fail\n", __LINE__);
      break;
    }

    printf("encrypto len: %d\n", length);
    // show_hex(cipher, 64);

    memset(plain, 0, sizeof(plain));

    length = aes_crypt(AES_DECRYPTO_MODE, cipher, length, plain);
    if (length < 0) {
      printf("[%d] error: aes_crypt fail\n", __LINE__);
      break;
    }

    printf("decrypto len: %d\n", length);
    // printf("%.*s\n", 64, plain);

    if (text_len == length && memcmp(plain, Text, length) == 0) {
      printf("\naes_cbc_test success\n");
    } else {
      printf("\naes_cbc_test fail\n");
    }

    ret = 0;
  } while (0);

  return ret;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_TIM1_Init();
  MX_AES_Init();
  /* USER CODE BEGIN 2 */

  EXTI->IMR1 |= 1<<0;
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&hlpuart1,rx1_buffer,BUFFER_SIZE);
  // HAL_TIM_Base_Start_IT(&htim1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  aes_ecb_test();
  aes_cbc_test();
  while (1)
  {
    if (task_scheduling(&task1, 500))
    {
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
