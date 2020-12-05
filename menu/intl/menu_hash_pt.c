/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2016 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <string.h>

#include <retro_assert.h>
#include <compat/strl.h>

#include "../menu_hash.h"
#include "../../configuration.h"

 /* IMPORTANT:
  * For non-english characters to work without proper unicode support,
  * we need this file to be encoded in ISO 8859-1 (Latin1), not UTF-8.
  * If you save this file as UTF-8, you'll break non-english characters
  * (e.g. German "Umlauts" and Portugese diacritics).
 */
/* DO NOT REMOVE THIS. If it causes build failure, it's because you saved the file as UTF-8. Read the above comment. */
extern const char force_iso_8859_1[sizeof("������������")==12+1 ? 1 : -1];

const char *menu_hash_to_str_pt(uint32_t hash)
{
   switch (hash)
   {
      case MENU_LABEL_VALUE_INFORMATION_LIST:
         return "Informa��o";
      case MENU_LABEL_VALUE_USE_BUILTIN_PLAYER:
         return "Usar Player Interno"; /* TODO/FIXME */
      case MENU_LABEL_VALUE_CONTENT_SETTINGS:
         return "Menu R�pido";
      case MENU_LABEL_VALUE_RDB_ENTRY_CRC32:
         return "CRC32";
      case MENU_LABEL_VALUE_RDB_ENTRY_MD5:
         return "MD5";
      case MENU_LABEL_VALUE_LOAD_CONTENT_LIST:
         return "Carregar Conte�do";
      case MENU_LABEL_VALUE_LOAD_ARCHIVE:
         return "Fechado";
      case MENU_LABEL_VALUE_OPEN_ARCHIVE:
         return "Aberto";
      case MENU_VALUE_ASK_ARCHIVE:
         return "Ask";
      case MENU_LABEL_VALUE_PRIVACY_SETTINGS:
         return "Configura��es de Privacidade";
      case MENU_VALUE_HORIZONTAL_MENU:
         return "Horizontal Menu"; /* FIXME - don't edit this yet. */
      case MENU_LABEL_VALUE_NO_SETTINGS_FOUND:
         return "Nenhuma defini��o encontrada.";
      case MENU_LABEL_VALUE_NO_PERFORMANCE_COUNTERS:
         return "Nenhum medidor de desempenho.";
      case MENU_LABEL_VALUE_DRIVER_SETTINGS:
         return "Drivers";
      case MENU_LABEL_VALUE_CONFIGURATION_SETTINGS:
         return "Configura��es";
      case MENU_LABEL_VALUE_CORE_SETTINGS:
         return "Core";
      case MENU_LABEL_VALUE_VIDEO_SETTINGS:
         return "V�deo";
      case MENU_LABEL_VALUE_LOGGING_SETTINGS:
         return "Registro de Dados";
      case MENU_LABEL_VALUE_SAVING_SETTINGS:
         return "Saves";
      case MENU_LABEL_VALUE_REWIND_SETTINGS:
         return "Retrocesso";
      case MENU_VALUE_SHADER:
         return "Shader";
      case MENU_VALUE_CHEAT:
         return "Cheat";
      case MENU_VALUE_USER:
         return "Usu�rio";
      case MENU_LABEL_VALUE_SYSTEM_BGM_ENABLE:
         return "Ativar Sistema BGM";
      case MENU_VALUE_RETROPAD:
         return "3DS";
      case MENU_VALUE_RETROKEYBOARD:
         return "RetroTeclado";
      case MENU_LABEL_VALUE_AUDIO_BLOCK_FRAMES:
         return "Quadros de Blocos de �udio";
      case MENU_LABEL_VALUE_INPUT_DESCRIPTOR_LABEL_SHOW:
         return "Mostrar R�tulos de Entradas de Core";
      case MENU_LABEL_VALUE_INPUT_DESCRIPTOR_HIDE_UNBOUND:
         return "Esconder Descritores de Entradas sem Uso";
      case MENU_LABEL_VALUE_VIDEO_FONT_ENABLE:
         return "Mostrar Mensagem de Tela";
      case MENU_LABEL_VALUE_VIDEO_FONT_PATH:
         return "Fonte da Mensagem de Tela";
      case MENU_LABEL_VALUE_VIDEO_FONT_SIZE:
         return "Tamanho da Mensagem de Tela";
      case MENU_LABEL_VALUE_VIDEO_MESSAGE_POS_X:
         return "Posi��o X da Mensagem de Tela";
      case MENU_LABEL_VALUE_VIDEO_MESSAGE_POS_Y:
         return "Posi��o Y da Mensagem de Tela";
      case MENU_LABEL_VALUE_VIDEO_SOFT_FILTER:
         return "Ativar Filtro de Suaviza��o";
      case MENU_LABEL_VALUE_VIDEO_FILTER_FLICKER:
         return "Filtro de Cintila��o";
      case MENU_VALUE_DIRECTORY_CONTENT:
         return "<Diret�rio de Conte�do>";
      case MENU_VALUE_UNKNOWN:
         return "Desconhecido";
      case MENU_VALUE_DONT_CARE:
         return "Tanto faz";
      case MENU_VALUE_LINEAR:
         return "Linear";
      case MENU_VALUE_NEAREST:
         return "Nearest";
      case MENU_VALUE_DIRECTORY_DEFAULT:
         return "<Padr�o>";
      case MENU_VALUE_DIRECTORY_NONE:
         return "<Nenhum>";
      case MENU_VALUE_NOT_AVAILABLE:
         return "N/A";
      case MENU_LABEL_VALUE_INPUT_REMAPPING_DIRECTORY:
         return "Remapeamentos de Controladores";
      case MENU_LABEL_VALUE_JOYPAD_AUTOCONFIG_DIR:
         return "Autoconfigura��es de Dispositivos de Entrada";
      case MENU_LABEL_VALUE_RECORDING_CONFIG_DIRECTORY:
         return "Configura��es de Grava��es";
      case MENU_LABEL_VALUE_RECORDING_OUTPUT_DIRECTORY:
         return "Grava��es";
      case MENU_LABEL_VALUE_SCREENSHOT_DIRECTORY:
         return "Capturas de Telas";
      case MENU_LABEL_VALUE_PLAYLIST_DIRECTORY:
         return "Hist�ricos";
      case MENU_LABEL_VALUE_SAVEFILE_DIRECTORY:
         return "Saves";
      case MENU_LABEL_VALUE_SAVESTATE_DIRECTORY:
         return "Savestates";
      case MENU_LABEL_VALUE_STDIN_CMD_ENABLE:
         return "Comandos stdin";
      case MENU_LABEL_VALUE_VIDEO_DRIVER:
         return "Driver de V�deo";
      case MENU_LABEL_VALUE_RECORD_ENABLE:
         return "Ativar Grava��o";
      case MENU_LABEL_VALUE_VIDEO_GPU_RECORD:
         return "Ativar Grava��o por GPU";
      case MENU_LABEL_VALUE_RECORD_PATH: /* FIXME/UPDATE */
         return "Caminho da Grava��o";
      case MENU_LABEL_VALUE_RECORD_USE_OUTPUT_DIRECTORY:
         return "Diret�rio de Sa�da";
      case MENU_LABEL_VALUE_RECORD_CONFIG:
         return "Configura��es de Grava��o";
      case MENU_LABEL_VALUE_VIDEO_POST_FILTER_RECORD:
         return "Ativar Filtro P�s-Grava��o";
      case MENU_LABEL_VALUE_CORE_ASSETS_DIRECTORY:
         return "Recursos (Assets) de Cores"; /* FIXME/UPDATE */
      case MENU_LABEL_VALUE_ASSETS_DIRECTORY:
         return "Recursos (Assets)";
      case MENU_LABEL_VALUE_DYNAMIC_WALLPAPERS_DIRECTORY:
         return "Pap�is de Parede Din�micos";
      case MENU_LABEL_VALUE_BOXARTS_DIRECTORY:
         return "Boxarts";
      case MENU_LABEL_VALUE_RGUI_BROWSER_DIRECTORY:
         return "Navega��o";
      case MENU_LABEL_VALUE_RGUI_CONFIG_DIRECTORY:
         return "Configura��es";
      case MENU_LABEL_VALUE_LIBRETRO_INFO_PATH:
         return "Informa��es de Cores";
      case MENU_LABEL_VALUE_LIBRETRO_DIR_PATH:
         return "Cores";
      case MENU_LABEL_VALUE_CURSOR_DIRECTORY:
         return "Cursores";
      case MENU_LABEL_VALUE_CONTENT_DATABASE_DIRECTORY:
         return "Databases de Conte�do";
      case MENU_LABEL_VALUE_SYSTEM_DIRECTORY:
         return "System/BIOS";
      case MENU_LABEL_VALUE_CHEAT_DATABASE_PATH:
         return "Cheats";
      case MENU_LABEL_VALUE_CACHE_DIRECTORY: /* UPDATE/FIXME */
         return "Descompacta��o";
      case MENU_LABEL_VALUE_AUDIO_FILTER_DIR:
         return "Filtros de �udio";
      case MENU_LABEL_VALUE_VIDEO_SHADER_DIR:
         return "Shaders";
      case MENU_LABEL_VALUE_VIDEO_FILTER_DIR:
         return "Filtros de V�deo";
      case MENU_LABEL_VALUE_OVERLAY_DIRECTORY:
         return "Overlays";
      case MENU_LABEL_VALUE_OSK_OVERLAY_DIRECTORY:
         return "Overlays de Teclado";
      case MENU_LABEL_VALUE_NETPLAY_CLIENT_SWAP_INPUT:
         return "Trocar Entradas de Netplay";
      case MENU_LABEL_VALUE_NETPLAY_SPECTATOR_MODE_ENABLE:
         return "Ativar Espectador de Netplay";
      case MENU_LABEL_VALUE_NETPLAY_IP_ADDRESS:
         return "Endere�o IP";
      case MENU_LABEL_VALUE_NETPLAY_TCP_UDP_PORT:
         return "Portas TCP/UDP de Netplay";
      case MENU_LABEL_VALUE_NETPLAY_ENABLE:
         return "Ativar Netplay";
      case MENU_LABEL_VALUE_NETPLAY_DELAY_FRAMES:
         return "Quadros de Retardo de Netplay";
      case MENU_LABEL_VALUE_NETPLAY_MODE:
         return "Ativar Cliente de Netplay";
      case MENU_LABEL_VALUE_RGUI_SHOW_START_SCREEN:
         return "Mostrar Tela de In�cio";
      case MENU_LABEL_VALUE_TITLE_COLOR:
         return "Cor do Menu T�tulo";
      case MENU_LABEL_VALUE_ENTRY_HOVER_COLOR:
         return "Cor de Realce do Menu Inicial";
      case MENU_LABEL_VALUE_TIMEDATE_ENABLE:
         return "Mostrar Hora / Data";
      case MENU_LABEL_VALUE_THREADED_DATA_RUNLOOP_ENABLE:
         return "Ativar Runloop de Threads de Dados";
      case MENU_LABEL_VALUE_ENTRY_NORMAL_COLOR:
         return "Cor do Menu Inicial";
      case MENU_LABEL_VALUE_SHOW_ADVANCED_SETTINGS:
         return "Mostrar Configura��es Avan�adas";
      case MENU_LABEL_VALUE_MOUSE_ENABLE:
         return "Suporte a Mouse";
      case MENU_LABEL_VALUE_POINTER_ENABLE:
         return "Suporte a Touch";
      case MENU_LABEL_VALUE_CORE_ENABLE:
         return "Mostrar Nome dos Cores";
      case MENU_LABEL_VALUE_DPI_OVERRIDE_ENABLE:
         return "Ativar Sobreposi��o de DPI";
      case MENU_LABEL_VALUE_DPI_OVERRIDE_VALUE:
         return "Sobreposi��o de DPI";
      case MENU_LABEL_VALUE_SUSPEND_SCREENSAVER_ENABLE:
         return "Suspender Prote��o de Tela";
      case MENU_LABEL_VALUE_VIDEO_DISABLE_COMPOSITION:
         return "Desativar Desktop Composition";
      case MENU_LABEL_VALUE_PAUSE_NONACTIVE:
         return "N�o Rodar em Background";
      case MENU_LABEL_VALUE_UI_COMPANION_START_ON_BOOT:
         return "Ativar UI Companion ao Iniciar";
      case MENU_LABEL_VALUE_UI_MENUBAR_ENABLE:
         return "Barra de Menu (Dica)";
      case MENU_LABEL_VALUE_ARCHIVE_MODE:
         return "A��o para Arquivos Compactados";
      case MENU_LABEL_VALUE_NETWORK_CMD_ENABLE:
         return "Comandos de Rede";
      case MENU_LABEL_VALUE_NETWORK_CMD_PORT:
         return "Porta para Comandos de Rede";
      case MENU_LABEL_VALUE_HISTORY_LIST_ENABLE:
         return "Ativar Lista de Hist�rico";
      case MENU_LABEL_VALUE_CONTENT_HISTORY_SIZE:
         return "Tamanho da Lista de Hist�rico";
      case MENU_LABEL_VALUE_VIDEO_REFRESH_RATE_AUTO:
         return "Taxa de Atualiza��o de Quadros Estimada";
      case MENU_LABEL_VALUE_DUMMY_ON_CORE_SHUTDOWN:
         return "Desligar Core Dummy On";
      case MENU_LABEL_VALUE_CORE_SET_SUPPORTS_NO_CONTENT_ENABLE: /* TODO/FIXME */
         return "N�o Iniciar Cores Automaticamente";
      case MENU_LABEL_VALUE_FRAME_THROTTLE_ENABLE:
         return "Limitar Velocidade M�xima de Execu��o";
      case MENU_LABEL_VALUE_FASTFORWARD_RATIO:
         return "Velocidade M�xima de Execu��o";
      case MENU_LABEL_VALUE_AUTO_REMAPS_ENABLE:
         return "Carregar Automaticamente Arquivos Remapeados";
      case MENU_LABEL_VALUE_SLOWMOTION_RATIO:
         return "Taxa de C�mera Lenta";
      case MENU_LABEL_VALUE_CORE_SPECIFIC_CONFIG:
         return "Configura��o por Core";
      case MENU_LABEL_VALUE_AUTO_OVERRIDES_ENABLE:
         return "Carregar Automaticamente Arquivos de Sobreposi��o";
      case MENU_LABEL_VALUE_CONFIG_SAVE_ON_EXIT:
         return "Salvar Configura��o ao Sair";
      case MENU_LABEL_VALUE_VIDEO_SMOOTH:
         return "Filtragem Bilinear por Hardware";
      case MENU_LABEL_VALUE_VIDEO_GAMMA:
         return "Gamma de V�deo";
      case MENU_LABEL_VALUE_VIDEO_ALLOW_ROTATE:
         return "Permitir Rota��o";
      case MENU_LABEL_VALUE_VIDEO_HARD_SYNC:
         return "Sincronizar GPU com CPU";
      case MENU_LABEL_VALUE_VIDEO_SWAP_INTERVAL:
         return "Intervalo de Permuta do Sincronismo Vertical";
      case MENU_LABEL_VALUE_VIDEO_VSYNC:
         return "Sincronismo Vertical";
      case MENU_LABEL_VALUE_VIDEO_THREADED:
         return "V�deo em Threads";
      case MENU_LABEL_VALUE_VIDEO_ROTATION:
         return "Rota��o";
      case MENU_LABEL_VALUE_VIDEO_GPU_SCREENSHOT:
         return "Ativar Captura de Tela via GPU";
      case MENU_LABEL_VALUE_VIDEO_CROP_OVERSCAN:
         return "Descartar Overscan (Recarregue)";
      case MENU_LABEL_VALUE_VIDEO_ASPECT_RATIO_INDEX:
         return "�ndice de Rela��es de Aspecto";
      case MENU_LABEL_VALUE_VIDEO_ASPECT_RATIO_AUTO:
         return "Rela��o de Aspecto Autom�tica";
      case MENU_LABEL_VALUE_VIDEO_FORCE_ASPECT:
         return "For�ar Rela��o de Aspecto";
      case MENU_LABEL_VALUE_VIDEO_REFRESH_RATE:
         return "Taxa de Atualiza��o de Tela";
      case MENU_LABEL_VALUE_VIDEO_FORCE_SRGB_DISABLE:
         return "Forcar Desativa��o de sRGB FBO";
      case MENU_LABEL_VALUE_VIDEO_WINDOWED_FULLSCREEN:
         return "Modo Tela Cheia em Janela";
      case MENU_LABEL_VALUE_PAL60_ENABLE:
         return "Usar Modo PAL60";
      case MENU_LABEL_VALUE_VIDEO_VFILTER:
         return "Eliminar Cintila��o";
      case MENU_LABEL_VALUE_VIDEO_VI_WIDTH:
         return "Definir Largura de Tela VI";
      case MENU_LABEL_VALUE_VIDEO_BLACK_FRAME_INSERTION:
         return "Inser��o de Quadro Negro";
      case MENU_LABEL_VALUE_VIDEO_HARD_SYNC_FRAMES:
         return "Quadros de Sincroniza��o entre GPU e CPU";
      case MENU_LABEL_VALUE_SORT_SAVEFILES_ENABLE:
         return "Ordenar Saves em Pastas";
      case MENU_LABEL_VALUE_SORT_SAVESTATES_ENABLE:
         return "Ordenar Savestates em Pastas";
      case MENU_LABEL_VALUE_VIDEO_FULLSCREEN:
         return "Usar Modo Tela Cheia";
      case MENU_LABEL_VALUE_VIDEO_SCALE:
         return "Variar Escala em Janela";
      case MENU_LABEL_VALUE_VIDEO_SCALE_INTEGER:
         return "Escala em Degraus Inteiros";
      case MENU_LABEL_VALUE_PERFCNT_ENABLE:
         return "Contadores de Desempenho";
      case MENU_LABEL_VALUE_LIBRETRO_LOG_LEVEL:
         return "N�vel de Registro de Core";
      case MENU_LABEL_VALUE_LOG_VERBOSITY:
         return "Detalhamento de Registro";
      case MENU_LABEL_VALUE_SAVESTATE_AUTO_LOAD:
         return "Carregar Savestate Automaticamente";
      case MENU_LABEL_VALUE_SAVESTATE_AUTO_INDEX:
         return "�ndice Autom�tico de Savestates";
      case MENU_LABEL_VALUE_SAVESTATE_AUTO_SAVE:
         return "Savestate Autom�tico";
      case MENU_LABEL_VALUE_AUTOSAVE_INTERVAL:
         return "Intervalo de Grava��o Autom�tica de SaveRAM";
      case MENU_LABEL_VALUE_BLOCK_SRAM_OVERWRITE:
         return "N�o Sobrescrever SaveRAM ao Carregar Savestate";
      case MENU_LABEL_VALUE_VIDEO_SHARED_CONTEXT:
         return "Ativar Contexto Compartilhado de Hardware";
      case MENU_LABEL_VALUE_RESTART_RETROARCH:
         return "Reiniciar RetroArch";
      case MENU_LABEL_VALUE_NETPLAY_NICKNAME:
         return "Nome de Usu�rio";
      case MENU_LABEL_VALUE_USER_LANGUAGE:
         return "Idioma";
      case MENU_LABEL_VALUE_CAMERA_ALLOW:
         return "Autorizar C�mera";
      case MENU_LABEL_VALUE_LOCATION_ALLOW:
         return "Autorizar Localiza��o";
      case MENU_LABEL_VALUE_PAUSE_LIBRETRO:
         return "Pausar Quando o Menu for Ativado";
      case MENU_LABEL_VALUE_INPUT_OSK_OVERLAY_ENABLE:
         return "Mostrar Overlay de Teclado";
      case MENU_LABEL_VALUE_INPUT_OVERLAY_ENABLE:
         return "Mostrar Overlay";
      case MENU_LABEL_VALUE_VIDEO_MONITOR_INDEX:
         return "�ndice de Monitores";
      case MENU_LABEL_VALUE_VIDEO_FRAME_DELAY:
         return "Retardo de Quadro";
      case MENU_LABEL_VALUE_INPUT_DUTY_CYCLE:
         return "Ciclo de Trabalho";
      case MENU_LABEL_VALUE_INPUT_TURBO_PERIOD:
         return "Per�odo de Turbo";
      case MENU_LABEL_VALUE_INPUT_AXIS_THRESHOLD:
         return "Limiar de Eixo do Controlador";
      case MENU_LABEL_VALUE_INPUT_REMAP_BINDS_ENABLE:
         return "Ativar Remapeamentos";
      case MENU_LABEL_VALUE_INPUT_MAX_USERS:
         return "Usu�rios M�ximos";
      case MENU_LABEL_VALUE_INPUT_AUTODETECT_ENABLE:
         return "Ativar Autoconfigura��o";
      case MENU_LABEL_VALUE_AUDIO_OUTPUT_RATE:
         return "Taxa de Amostragem de �udio (KHz)";
      case MENU_LABEL_VALUE_AUDIO_MAX_TIMING_SKEW:
         return "Distor��o M�xima de Sincroniza��o de �udio";
      case MENU_LABEL_VALUE_CHEAT_NUM_PASSES:
         return "C�digos de Cheat";
      case MENU_LABEL_VALUE_REMAP_FILE_SAVE_CORE:
         return "Salvar Remapeamento de Core";
      case MENU_LABEL_VALUE_REMAP_FILE_SAVE_GAME:
         return "Salvar Remapeamento de Jogo";
      case MENU_LABEL_VALUE_REMAP_FILE_RESET_DEFAULTS:
         return "Reset Defaults";
      case MENU_LABEL_VALUE_CHEAT_APPLY_CHANGES:
         return "Aplicar Altera��es de Cheats";
      case MENU_LABEL_VALUE_SHADER_APPLY_CHANGES:
         return "Aplicar Altera��es de Shaders";
      case MENU_LABEL_VALUE_REWIND_ENABLE:
         return "Ativar Retrocesso";
      case MENU_LABEL_VALUE_CONTENT_COLLECTION_LIST:
         return "Selecionar da Cole��o";
      case MENU_LABEL_VALUE_DETECT_CORE_LIST:
         return "Selecionar Arquivo e Detectar Core";
      case MENU_LABEL_VALUE_LOAD_CONTENT_HISTORY:
         return "Selecionar do Hist�rico";
      case MENU_LABEL_VALUE_AUDIO_ENABLE:
         return "Ativar �udio";
      case MENU_LABEL_VALUE_FPS_SHOW:
         return "Mostrar Taxa de Quadros";
      case MENU_LABEL_VALUE_AUDIO_MUTE:
         return "Silenciar �udio";
      case MENU_LABEL_VALUE_AUDIO_VOLUME:
         return "Volume de �udio (dB)";
      case MENU_LABEL_VALUE_AUDIO_SYNC:
         return "Ativar Sincronismo de �udio";
      case MENU_LABEL_VALUE_AUDIO_RATE_CONTROL_DELTA:
         return "Varia��o M�xima de Taxa de �udio";
      case MENU_LABEL_VALUE_VIDEO_SHADER_NUM_PASSES:
         return "N�mero de Shaders";
      case MENU_LABEL_VALUE_RDB_ENTRY_SHA1:
         return "SHA1";
      case MENU_LABEL_VALUE_CONFIGURATIONS:
         return "Carregar Configura��o";
      case MENU_LABEL_VALUE_REWIND_GRANULARITY:
         return "Granularidade do Retrocesso";
      case MENU_LABEL_VALUE_REMAP_FILE_LOAD:
         return "Carregar Remapeamento";
      case MENU_LABEL_VALUE_CUSTOM_RATIO:
         return "Rela��o de Aspecto Personalizada";
      case MENU_LABEL_VALUE_USE_THIS_DIRECTORY:
         return "<Usar este diret�rio>";
      case MENU_LABEL_VALUE_RDB_ENTRY_START_CONTENT:
         return "Iniciar Conte�do";
      case MENU_LABEL_VALUE_DISK_OPTIONS:
         return "Op��es de Disco do Core";
      case MENU_LABEL_VALUE_CORE_OPTIONS:
         return "Op��es do Core";
      case MENU_LABEL_VALUE_CORE_CHEAT_OPTIONS:
         return "Op��es de Cheat do Core";
      case MENU_LABEL_VALUE_CHEAT_FILE_LOAD:
         return "Carregar Cheat";
      case MENU_LABEL_VALUE_CHEAT_FILE_SAVE_AS:
         return "Salvar Cheat Como";
      case MENU_LABEL_VALUE_CORE_COUNTERS:
         return "Contadores de Core";
      case MENU_LABEL_VALUE_TAKE_SCREENSHOT:
         return "Capturar Tela";
      case MENU_LABEL_VALUE_RESUME:
         return "Retomar";
      case MENU_LABEL_VALUE_DISK_INDEX:
         return "�ndice de Discos";
      case MENU_LABEL_VALUE_FRONTEND_COUNTERS:
         return "Contadores do Frontend";
      case MENU_LABEL_VALUE_DISK_IMAGE_APPEND:
         return "Anexar Imagem de Disco";
      case MENU_LABEL_VALUE_DISK_CYCLE_TRAY_STATUS:
         return "Estado do Drive de Disco";
      case MENU_LABEL_VALUE_NO_PLAYLIST_ENTRIES_AVAILABLE:
         return "Hist�rico vazio.";
      case MENU_LABEL_VALUE_NO_CORE_INFORMATION_AVAILABLE:
         return "Nenhuma informa��o de core dispon�vel.";
      case MENU_LABEL_VALUE_NO_CORE_OPTIONS_AVAILABLE:
         return "Nenhuma op��o de core dispon�vel.";
      case MENU_LABEL_VALUE_NO_CORES_AVAILABLE:
         return "Nenhum core dispon�vel.";
      case MENU_VALUE_NO_CORE:
         return "Nenhum Core";
      case MENU_LABEL_VALUE_DATABASE_MANAGER:
         return "Gerenciador de Databases";
      case MENU_LABEL_VALUE_CURSOR_MANAGER:
         return "Gerenciador de Cursores";
      case MENU_VALUE_MAIN_MENU:
         return "Main Menu";
      case MENU_LABEL_VALUE_SETTINGS:
         return "Defini��es";
      case MENU_LABEL_VALUE_QUIT_RETROARCH:
         return "Sair do RetroArch";
      case MENU_LABEL_VALUE_HELP:
         return "Ajuda";
      case MENU_LABEL_VALUE_SAVE_NEW_CONFIG:
         return "Salvar Nova Configura��o";
      case MENU_LABEL_VALUE_RESTART_CONTENT:
         return "Reiniciar Conte�do";
      case MENU_LABEL_VALUE_CORE_UPDATER_LIST:
         return "Atualiza��o de Cores";
      case MENU_LABEL_VALUE_CORE_UPDATER_BUILDBOT_URL:
         return "URL Buildbot de Cores";
      case MENU_LABEL_VALUE_BUILDBOT_ASSETS_URL:
         return "URL Buildbot de Recursos (Assets)";
      case MENU_LABEL_VALUE_NAVIGATION_WRAPAROUND:
         return "Navega��o Circular";
      case MENU_LABEL_VALUE_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE:
         return "Filtrar por Extens�es Suportadas";
      case MENU_LABEL_VALUE_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE:
         return "Autoextrair Arquivos Baixados";
      case MENU_LABEL_VALUE_SYSTEM_INFORMATION:
         return "Informa��o de Sistema";
      case MENU_LABEL_VALUE_ONLINE_UPDATER:
         return "Atualiza��o Online";
      case MENU_LABEL_VALUE_CORE_INFORMATION:
         return "Informa��o de Core";
      case MENU_LABEL_VALUE_DIRECTORY_NOT_FOUND:
         return "Diret�rio n�o encontrado.";
      case MENU_LABEL_VALUE_NO_ITEMS:
         return "Nenhum item.";
      case MENU_LABEL_VALUE_CORE_LIST:
         return "Carregar Core";
      case MENU_LABEL_VALUE_LOAD_CONTENT:
         return "Selecionar Arquivo";
      case MENU_LABEL_VALUE_CLOSE_CONTENT:
         return "Fechar Conte�do";
      case MENU_LABEL_VALUE_MANAGEMENT:
         return "Databases";
      case MENU_LABEL_VALUE_SAVE_STATE:
         return "Salvar Savestate";
      case MENU_LABEL_VALUE_LOAD_STATE:
         return "Carregar Savestate";
      case MENU_LABEL_VALUE_RESUME_CONTENT:
         return "Retomar Conte�do";
      case MENU_LABEL_VALUE_INPUT_DRIVER:
         return "Driver de Controlador";
      case MENU_LABEL_VALUE_AUDIO_DRIVER:
         return "Driver de �udio";
      case MENU_LABEL_VALUE_JOYPAD_DRIVER:
         return "Driver de Joypad";
      case MENU_LABEL_VALUE_AUDIO_RESAMPLER_DRIVER:
         return "Driver do Amostrador de �udio";
      case MENU_LABEL_VALUE_RECORD_DRIVER:
         return "Driver de Grava��o";
      case MENU_LABEL_VALUE_MENU_DRIVER:
         return "Driver de Menu";
      case MENU_LABEL_VALUE_CAMERA_DRIVER:
         return "Driver de C�mera";
      case MENU_LABEL_VALUE_LOCATION_DRIVER:
         return "Driver de Localiza��o";
      case MENU_LABEL_VALUE_UNABLE_TO_READ_COMPRESSED_FILE:
         return "Incapaz de ler arquivo compactado.";
      case MENU_LABEL_VALUE_OVERLAY_SCALE:
         return "Interpola��o de Overlay";
      case MENU_LABEL_VALUE_OVERLAY_PRESET:
         return "Predefini��es de Overlay";
      case MENU_LABEL_VALUE_AUDIO_LATENCY:
         return "Lat�ncia de �udio (ms)";
      case MENU_LABEL_VALUE_AUDIO_DEVICE:
         return "Dispositivo de �udio";
      case MENU_LABEL_VALUE_KEYBOARD_OVERLAY_PRESET:
         return "Predefini��es de Overlay de Teclado";
      case MENU_LABEL_VALUE_OVERLAY_OPACITY:
         return "Opacidade de Overlay";
      case MENU_LABEL_VALUE_MENU_WALLPAPER:
         return "Papel de Parede do Menu";
      case MENU_LABEL_VALUE_DYNAMIC_WALLPAPER:
         return "Papel de Parede Din�mico";
      case MENU_LABEL_VALUE_BOXART:
         return "Mostrar Boxart";
      case MENU_LABEL_VALUE_CORE_INPUT_REMAPPING_OPTIONS: /* TODO/FIXME */
         return "Op��es de Remapeamento de Controlador de Core";
      case MENU_LABEL_VALUE_SHADER_OPTIONS:
         return "Op��es de Shaders";
      case MENU_LABEL_VALUE_VIDEO_SHADER_PARAMETERS:
         return "Par�metros de Shader em Uso";
      case MENU_LABEL_VALUE_VIDEO_SHADER_PRESET_PARAMETERS:
         return "Menu de Par�metros de Shader";
      case MENU_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_AS:
         return "Salvar Predefini��es de Shader Como";
      case MENU_LABEL_VALUE_NO_SHADER_PARAMETERS:
         return "Nenhum par�metro de shader dispon�vel.";
      case MENU_LABEL_VALUE_VIDEO_SHADER_PRESET:
         return "Carregar Predefini��es de Shader";
      case MENU_LABEL_VALUE_VIDEO_FILTER:
         return "Filtro de V�deo";
      case MENU_LABEL_VALUE_AUDIO_DSP_PLUGIN:
         return "Plugin de DSP de �udio";
      case MENU_LABEL_VALUE_STARTING_DOWNLOAD:
         return "Iniciando download: ";
      case MENU_VALUE_SECONDS:
         return "segundos";
      case MENU_VALUE_OFF:
         return "OFF";
      case MENU_VALUE_ON:
         return "ON";
      case MENU_LABEL_VALUE_UPDATE_ASSETS:
         return "Atualizar Recursos (Assets)";
      case MENU_LABEL_VALUE_UPDATE_CHEATS:
         return "Atualizar Cheats";
      case MENU_LABEL_VALUE_UPDATE_AUTOCONFIG_PROFILES:
         return "Atualizar Perfis de Autoconfigura��o";
      case MENU_LABEL_VALUE_UPDATE_DATABASES:
         return "Atualizar Databases";
      case MENU_LABEL_VALUE_UPDATE_OVERLAYS:
         return "Atualizar Overlays";
      case MENU_LABEL_VALUE_UPDATE_CG_SHADERS:
         return "Atualizar Shaders Cg";
      case MENU_LABEL_VALUE_UPDATE_GLSL_SHADERS:
         return "Atualizar Shaders GLSL";
      case MENU_LABEL_VALUE_CORE_INFO_CORE_NAME:
         return "Nome do core";
      case MENU_LABEL_VALUE_CORE_INFO_CORE_LABEL:
         return "R�tulo do core";
      case MENU_LABEL_VALUE_CORE_INFO_SYSTEM_NAME:
         return "Nome do sistema";
      case MENU_LABEL_VALUE_CORE_INFO_SYSTEM_MANUFACTURER:
         return "Fabricante do sistema";
      case MENU_LABEL_VALUE_CORE_INFO_CATEGORIES:
         return "Categorias";
      case MENU_LABEL_VALUE_CORE_INFO_AUTHORS:
         return "Autores";
      case MENU_LABEL_VALUE_CORE_INFO_PERMISSIONS:
         return "Permiss�es";
      case MENU_LABEL_VALUE_CORE_INFO_LICENSES:
         return "Licen�a(s)";
      case MENU_LABEL_VALUE_CORE_INFO_SUPPORTED_EXTENSIONS:
         return "Extens�es suportadas";
      case MENU_LABEL_VALUE_CORE_INFO_FIRMWARE:
         return "Firmware";
      case MENU_LABEL_VALUE_CORE_INFO_CORE_NOTES:
         return "Notas do core";
      case MENU_LABEL_VALUE_SYSTEM_INFO_BUILD_DATE:
         return "Data do build";
      case MENU_LABEL_VALUE_SYSTEM_INFO_GIT_VERSION:
         return "Vers�o do git";
      case MENU_LABEL_VALUE_SYSTEM_INFO_CPU_FEATURES:
         return "Atributos da CPU";
      case MENU_LABEL_VALUE_SYSTEM_INFO_FRONTEND_IDENTIFIER:
         return "Indentificador do frontend";
      case MENU_LABEL_VALUE_SYSTEM_INFO_FRONTEND_NAME:
         return "Nome do frontend";
      case MENU_LABEL_VALUE_SYSTEM_INFO_FRONTEND_OS:
         return "OS do frontend";
      case MENU_LABEL_VALUE_SYSTEM_INFO_RETRORATING_LEVEL:
         return "N�vel de RetroRating";
      case MENU_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE:
         return "Fonte de energia";
      case MENU_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_NO_SOURCE:
         return "Nenhuma fonte";
      case MENU_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_CHARGING:
         return "Carregando";
      case MENU_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_CHARGED:
         return "Carregado";
      case MENU_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_DISCHARGING:
         return "Descarregando";
      case MENU_LABEL_VALUE_SYSTEM_INFO_VIDEO_CONTEXT_DRIVER:
         return "Driver de contexto de v�deo";
      case MENU_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_WIDTH:
         return "Mostrar largura (mm)";
      case MENU_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_HEIGHT:
         return "Mostrar altura (mm)";
      case MENU_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_DPI:
         return "Mostrar DPI";
      case MENU_LABEL_VALUE_SYSTEM_INFO_LIBRETRODB_SUPPORT:
         return "Suporte a LibretroDB";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OVERLAY_SUPPORT:
         return "Suporte a Overlay";
      case MENU_LABEL_VALUE_SYSTEM_INFO_COMMAND_IFACE_SUPPORT:
         return "Suporte a interface de comandos";
      case MENU_LABEL_VALUE_SYSTEM_INFO_NETWORK_COMMAND_IFACE_SUPPORT:
         return "Suporte a interface de comandos de rede";
      case MENU_LABEL_VALUE_SYSTEM_INFO_COCOA_SUPPORT:
         return "Suporte a Cocoa";
      case MENU_LABEL_VALUE_SYSTEM_INFO_RPNG_SUPPORT:
         return "Suporte a PNG (RPNG)";
      case MENU_LABEL_VALUE_SYSTEM_INFO_SDL_SUPPORT:
         return "Suporte a SDL1.2";
      case MENU_LABEL_VALUE_SYSTEM_INFO_SDL2_SUPPORT:
         return "Suporte a SDL2";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OPENGL_SUPPORT:
         return "Suporte a OpenGL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OPENGLES_SUPPORT:
         return "Suporte a OpenGL ES";
      case MENU_LABEL_VALUE_SYSTEM_INFO_THREADING_SUPPORT:
         return "Suporte a Threading";
      case MENU_LABEL_VALUE_SYSTEM_INFO_KMS_SUPPORT:
         return "Suporte a KMS/EGL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_UDEV_SUPPORT:
         return "Suporte a Udev";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OPENVG_SUPPORT:
         return "Suporte a OpenVG";
      case MENU_LABEL_VALUE_SYSTEM_INFO_EGL_SUPPORT:
         return "Suporte a EGL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_X11_SUPPORT:
         return "Suporte a X11";
      case MENU_LABEL_VALUE_SYSTEM_INFO_WAYLAND_SUPPORT:
         return "Suporte a Wayland";
      case MENU_LABEL_VALUE_SYSTEM_INFO_XVIDEO_SUPPORT:
         return "Suporte a XVideo";
      case MENU_LABEL_VALUE_SYSTEM_INFO_ALSA_SUPPORT:
         return "Suporte a ALSA";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OSS_SUPPORT:
         return "Suporte a OSS";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OPENAL_SUPPORT:
         return "Suporte a OpenAL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_OPENSL_SUPPORT:
         return "Suporte a OpenSL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_RSOUND_SUPPORT:
         return "Suporte a RSound";
      case MENU_LABEL_VALUE_SYSTEM_INFO_ROARAUDIO_SUPPORT:
         return "Suporte a RoarAudio";
      case MENU_LABEL_VALUE_SYSTEM_INFO_JACK_SUPPORT:
         return "Suporte a JACK";
      case MENU_LABEL_VALUE_SYSTEM_INFO_PULSEAUDIO_SUPPORT:
         return "Suporte a PulseAudio";
      case MENU_LABEL_VALUE_SYSTEM_INFO_DSOUND_SUPPORT:
         return "Suporte a DirectSound";
      case MENU_LABEL_VALUE_SYSTEM_INFO_XAUDIO2_SUPPORT:
         return "Suporte a XAudio2";
      case MENU_LABEL_VALUE_SYSTEM_INFO_ZLIB_SUPPORT:
         return "Suporte a Zlib";
      case MENU_LABEL_VALUE_SYSTEM_INFO_7ZIP_SUPPORT:
         return "Suporte a 7zip";
      case MENU_LABEL_VALUE_SYSTEM_INFO_DYLIB_SUPPORT:
         return "Suporte a bibliotecas din�micas";
      case MENU_LABEL_VALUE_SYSTEM_INFO_CG_SUPPORT:
         return "Suporte a Cg";
      case MENU_LABEL_VALUE_SYSTEM_INFO_GLSL_SUPPORT:
         return "Suporte a GLSL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_HLSL_SUPPORT:
         return "Suporte a HLSL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_LIBXML2_SUPPORT:
         return "Suporte a an�lise XML libxml2";
      case MENU_LABEL_VALUE_SYSTEM_INFO_SDL_IMAGE_SUPPORT:
         return "Suporte a imagem SDL";
      case MENU_LABEL_VALUE_SYSTEM_INFO_FBO_SUPPORT:
         return "Suporte a OpenGL/Direct3D render-to-texture (multi-pass shaders)";
      case MENU_LABEL_VALUE_SYSTEM_INFO_FFMPEG_SUPPORT:
         return "Suporte a FFmpeg";
      case MENU_LABEL_VALUE_SYSTEM_INFO_CORETEXT_SUPPORT:
         return "Suporte a CoreText";
      case MENU_LABEL_VALUE_SYSTEM_INFO_FREETYPE_SUPPORT:
         return "Suporte a FreeType";
      case MENU_LABEL_VALUE_SYSTEM_INFO_NETPLAY_SUPPORT:
         return "Suporte a Netplay (peer-to-peer)";
      case MENU_LABEL_VALUE_SYSTEM_INFO_PYTHON_SUPPORT:
         return "Suporte a Python (script em shaders)";
      case MENU_LABEL_VALUE_SYSTEM_INFO_V4L2_SUPPORT:
         return "Suporte a Video4Linux2";
      case MENU_LABEL_VALUE_SYSTEM_INFO_LIBUSB_SUPPORT:
         return "Suporte a Libusb";
      case MENU_LABEL_VALUE_YES:
         return "Sim";
      case MENU_LABEL_VALUE_NO:
         return "N�o";
      case MENU_VALUE_BACK:
         return "VOLTAR";
      case MENU_LABEL_VALUE_SCREEN_RESOLUTION:
         return "Resolu��o de Tela";
      case MENU_VALUE_DISABLED:
         return "Desativado";
      case MENU_VALUE_PORT:
         return "Porta";
      case MENU_VALUE_NONE:
         return "Nenhum";
      case MENU_LABEL_VALUE_RDB_ENTRY_DEVELOPER:
         return "Desenvolvedor";
      case MENU_LABEL_VALUE_RDB_ENTRY_PUBLISHER:
         return "Editora";
      case MENU_LABEL_VALUE_RDB_ENTRY_DESCRIPTION:
         return "Descri��o";
      case MENU_LABEL_VALUE_RDB_ENTRY_NAME:
         return "Nome";
      case MENU_LABEL_VALUE_RDB_ENTRY_ORIGIN:
         return "Origem";
      case MENU_LABEL_VALUE_RDB_ENTRY_FRANCHISE:
         return "Franquia";
      case MENU_LABEL_VALUE_RDB_ENTRY_RELEASE_MONTH:
         return "M�s de Lan�amento";
      case MENU_LABEL_VALUE_RDB_ENTRY_RELEASE_YEAR:
         return "Ano de Lan�amento";
      case MENU_VALUE_TRUE:
         return "Verdadeiro";
      case MENU_VALUE_FALSE:
         return "Falso";
      case MENU_VALUE_MISSING:
         return "Faltando";
      case MENU_VALUE_PRESENT:
         return "Presente";
      case MENU_VALUE_OPTIONAL:
         return "Opcional";
      case MENU_VALUE_REQUIRED:
         return "Obrigat�rio";
      case MENU_VALUE_STATUS:
         return "Status";
      case MENU_LABEL_VALUE_AUDIO_SETTINGS:
         return "�udio";
      case MENU_LABEL_VALUE_INPUT_SETTINGS:
         return "Entradas";
      case MENU_LABEL_VALUE_ONSCREEN_DISPLAY_SETTINGS:
         return "Informa��es de Tela";
      case MENU_LABEL_VALUE_OVERLAY_SETTINGS:
         return "Overlay em Tela";
      case MENU_LABEL_VALUE_MENU_SETTINGS:
         return "Menu";
      case MENU_LABEL_VALUE_MULTIMEDIA_SETTINGS:
         return "Reprodu��o de M�dia"; /* UPDATE/FIXME */
      case MENU_LABEL_VALUE_UI_SETTINGS:
         return "Interface de Usu�rio";
      case MENU_LABEL_VALUE_MENU_FILE_BROWSER_SETTINGS:
         return "Menu de Navega��o";
      case MENU_LABEL_VALUE_CORE_UPDATER_SETTINGS:
         return "Atualiza��o de Core"; /* UPDATE/FIXME */
      case MENU_LABEL_VALUE_NETWORK_SETTINGS:
         return "Rede";
      case MENU_LABEL_VALUE_PLAYLIST_SETTINGS:
         return "Hist�rico";
      case MENU_LABEL_VALUE_USER_SETTINGS:
         return "Usu�rio";
      case MENU_LABEL_VALUE_DIRECTORY_SETTINGS:
         return "Diret�rios";
      case MENU_LABEL_VALUE_RECORDING_SETTINGS:
         return "Grava��o";
      case MENU_LABEL_VALUE_NO_INFORMATION_AVAILABLE:
         return "Nenhuma informa��o dispon�vel.";
      case MENU_LABEL_VALUE_INPUT_USER_BINDS:
         return "Usu�rio %u";
      case MENU_VALUE_LANG_ENGLISH:
         return "Ingl�s";
      case MENU_VALUE_LANG_JAPANESE:
         return "Japon�s";
      case MENU_VALUE_LANG_FRENCH:
         return "Franc�s";
      case MENU_VALUE_LANG_SPANISH:
         return "Espanhol";
      case MENU_VALUE_LANG_GERMAN:
         return "Alem�o";
      case MENU_VALUE_LANG_ITALIAN:
         return "Italiano";
      case MENU_VALUE_LANG_DUTCH:
         return "Holand�s";
      case MENU_VALUE_LANG_PORTUGUESE:
         return "Portugu�s";
      case MENU_VALUE_LANG_RUSSIAN:
         return "Russo";
      case MENU_VALUE_LANG_KOREAN:
         return "Coreano";
      case MENU_VALUE_LANG_CHINESE_TRADITIONAL:
         return "Chin�s (Tradicional)";
      case MENU_VALUE_LANG_CHINESE_SIMPLIFIED:
         return "Chin�s (Simplificado)";
      case MENU_VALUE_LANG_ESPERANTO:
         return "Esperanto";
      case MENU_VALUE_LEFT_ANALOG:
         return "Anal�gico Esquerdo";
      case MENU_VALUE_RIGHT_ANALOG:
         return "Anal�gico Direito";
      case MENU_LABEL_VALUE_INPUT_HOTKEY_BINDS:
         return "Associa��o de Teclas de Atalho";
      case MENU_LABEL_VALUE_FRAME_THROTTLE_SETTINGS:
         return "Defini��es do Limitador de Quadros";
      case MENU_VALUE_SEARCH:
         return "Busca:";
      case MENU_LABEL_VALUE_USE_BUILTIN_IMAGE_VIEWER:
         return "Usar Visualizador de Imagens Interno";
      default:
         break;
   }
 
   return "null";
}

int menu_hash_get_help_pt(uint32_t hash, char *s, size_t len)
{
   uint32_t driver_hash = 0;
   settings_t      *settings = config_get_ptr();

   /* If this one throws errors, stop sledgehammering square pegs into round holes and */
   /* READ THE COMMENTS at the top of the file. */ (void)sizeof(force_iso_8859_1);

   switch (hash)
   {
      case MENU_LABEL_INPUT_DRIVER:
         driver_hash = menu_hash_calculate(settings->input.driver);

         switch (driver_hash)
         {
            case MENU_LABEL_INPUT_DRIVER_UDEV:
               snprintf(s, len,
                     "Driver de entrada udev. \n"
                     " \n"
                     "Esse driver funciona sem o X. \n"
                     " \n"
                     "Ele usa a recente API de joypads \n"
                     "evdev para supporte a joystick. \n"
                     "Suporta Hot-Swap e force feedback \n"
                     "(se suportado pelo dispositivo). \n"
                     " \n"
                     "O driver l� os eventos evdev para suporte a \n"
                     "teclado. Suporta tamb�m callback de teclado, \n"
                     "mouses e touchpads. \n"
                     " \n"
                     "Em geral, na maioria das distribui��es, os n�s \n"
                     "/dev/input s�o root-only (modo 600). Mas voc� pode \n"
                     "definir uma regra udev para dar acesso a non-roots."
                     );
               break;
            case MENU_LABEL_INPUT_DRIVER_LINUXRAW:
               snprintf(s, len,
                     "Driver de Entrada linuxraw. \n"
                     " \n"
                     "Esse driver requer um TTY ativo. Eventos de \n"
                     "teclado s�o lidos diretamente do TTY, tornando-o \n"
                     "simples, mas n�o t�o flex�vel quanto o udev. \n" "Mouses, etc, n�o s�o suportados. \n"
                     " \n"
                     "Esse driver usa a antiga API de joysticks \n"
                     "(/dev/input/js*).");
               break;
            default:
               snprintf(s, len,
                     "Driver de Entrada.\n"
                     " \n"
                     "Dependendo do driver de v�deo, pode ser necess�rio \n"
                     "for�ar um driver de entrada diferente.");
               break;
         }
         break;
      case MENU_LABEL_LOAD_CONTENT:
         snprintf(s, len,
               "Carregar Conte�do. \n"
               "Busca conte�do. \n"
               " \n"
               "Para carregar conte�do, voc� precisa de \n"
               "um core libretro para usar, e um arquivo \n"
               "de conte�do. \n"
               " \n"
               "Para controlar onde o menu come�a a \n"
               "buscar conte�do, defina o Diret�rio \n"
               "de Navega��o. Se n�o estiver definido, \n"
               "o Retroarch come�ar� no diret�rio raiz. \n"
               " \n"
               "O navegador vai filtrar pelas extens�es \n"
               "do mais recente core definido em 'Core', \n"
               "e o usar� quando o conte�do estiver \n"
               "carregado."
               );
         break;
      case MENU_LABEL_CORE_LIST:
         snprintf(s, len,
               "Carregar Core. \n"
               " \n"
               "Busca uma implementa��o de um core \n"
               "libretro. Onde a busca inicia depende \n"
               "do caminho do seu Diret�rio de Cores. \n"
               "Se n�o definido, come�ar� no raiz. \n"
               " \n"
               "Se o Diret�rio de Cores estiver definido, \n"
               "o menu o usar� como pasta inicial. Se for um \n"
               "caminho completo, ele come�ar� na pasta onde \n"
               "o arquivo estiver.");
         break;
      case MENU_LABEL_LOAD_CONTENT_HISTORY:
         snprintf(s, len,
               "Carregando conte�do do hist�rico. \n"
               " \n"
               "Ao carregar conte�dos, suas combina��es com \n"
               "cores s�o salvas no hist�rico. \n"
               " \n"
               "O hist�rico � salvo em um arquivo no mesmo \n"
               "diret�rio do arquivo de configura��o. Se nenhuma \n"
               "configura��o tiver sido carregada, o hist�rico \n"
               "n�o ser� salvo ou carregado e n�o vai existir no \n"
               "menu principal."
               );
         break;
      case MENU_LABEL_VIDEO_DRIVER:
         driver_hash = menu_hash_calculate(settings->video.driver);

         switch (driver_hash)
         {
            case MENU_LABEL_VIDEO_DRIVER_GL:
               snprintf(s, len,
                     "Driver de V�deo OpenGL. \n"
                     " \n"
                     "Esse driver permite o uso de cores libretro GL  \n"
                     "em adi��o �s implementa��es de cores de \n"
                     "renderiza��o por software.\n"
                     " \n"
                     "O desempenho das implementa��es dos cores de\n"
                     "renderiza��o por software e libretro GL \n"
                     "depende do driver GL instalado em sua \n"
                     "placa de v�deo.");
               break;
            case MENU_LABEL_VIDEO_DRIVER_SDL2:
               snprintf(s, len,
                     "Driver de V�deo SDL 2.\n"
                     " \n"
                     "Esse � um driver de v�deo SDL 2 de \n"
                     "renderiza��o por software.\n"
                     " \n"
                     "O desempenho das implementa��es dos cores de \n"
                     "renderiza��o por software depende da \n"
                     "implementa��o SDL de sua plataforma.");
               break;
            case MENU_LABEL_VIDEO_DRIVER_SDL1:
               snprintf(s, len,
                     "Driver de V�deo SDL.\n"
                     " \n"
                     "Esse � um driver de v�deo SDL 1.2 de \n"
                     "renderiza��o por software.\n"
                     " \n"
                     "O desemprenho � considerado sub�timo. \n"
                     "Considere seu uso apenas em �ltimo caso.");
               break;
            case MENU_LABEL_VIDEO_DRIVER_D3D:
               snprintf(s, len,
                     "Driver de V�deo Direct3D. \n"
                     " \n"
                     "O desempenho das implementa��es dos cores de\n"
                     "renderiza��o por software depende do driver \n"
                     "D3D instalado em sua placa de v�deo.");
               break;
            case MENU_LABEL_VIDEO_DRIVER_EXYNOS:
               snprintf(s, len,
                     "Driver de V�deo Exynos-G2D. \n"
                     " \n"
                     "Esse � um driver de v�deo Exynos de baixo n�vel. \n"
                     "Usa o bloco G2D do SoC Samsung Exynos \n"
                     "para opera��es de blit. \n"
                     " \n"
                     "O desempenho para cores de renderiza��o por \n"
                     "software deve ser �timo.");
               break;
            case MENU_LABEL_VIDEO_DRIVER_SUNXI:
               snprintf(s, len,
                     "Driver de V�deo Sunxi-G2D. \n"
                     " \n"
                     "Esse � um driver de v�deo Sunxi de baixo n�vel. \n"
                     "Usa o bloco G2D dos SoCs Allwinner.");
               break;
            default:
               snprintf(s, len,
                     "Driver de V�deo em uso.");
               break;
         }
         break;
      case MENU_LABEL_AUDIO_DSP_PLUGIN:
         snprintf(s, len,
               "Plugin de DSP de �udio.\n"
               "Processa �udio antes de ser enviado ao \n"
               "driver."
               );
         break;
      case MENU_LABEL_AUDIO_RESAMPLER_DRIVER:
         driver_hash = menu_hash_calculate(settings->audio.resampler);

         switch (driver_hash)
         {
            case MENU_LABEL_AUDIO_RESAMPLER_DRIVER_SINC:
               snprintf(s, len,
                     "Implementa��o Windowed SINC.");
               break;
            case MENU_LABEL_AUDIO_RESAMPLER_DRIVER_CC:
               snprintf(s, len,
                     "Implementa��o Convoluted Cosine.");
               break;
         }
         break;
      case MENU_LABEL_VIDEO_SHADER_PRESET:
         snprintf(s, len,
               "Carregar Predefini��es de Shader. \n"
               " \n"
               " Carregar predefini��es em "
#ifdef HAVE_CG
               "Cg"
#endif
#ifdef HAVE_GLSL
#ifdef HAVE_CG
               "/"
#endif
               "GLSL"
#endif
#ifdef HAVE_HLSL
#if defined(HAVE_CG) || defined(HAVE_HLSL)
               "/"
#endif
               "HLSL"
#endif
               " diretamente. \n"
               "O menu de shaders � atualizado de acordo. \n"
               " \n"
               "Se o CGP usar m�todos de interpola��o complexos, \n"
               "(fator de escala diferente para X e Y) o fator \n"
               "de escala mostrado no menu poder� n�o ser \n"
               "correto."
               );
         break;
      case MENU_LABEL_VIDEO_SHADER_SCALE_PASS:
         snprintf(s, len,
               "Escala para este passo. \n"
               " \n"
               "O fator de escala se acumula, i.e. 2x \n"
               "para o primeiro passo e 2x para o segundo \n"
               "vai lhe fornecer uma escala total de 4x. \n"
               " \n"
               "Se houver um fator de escala no �ltimo \n"
               "passo, o resultado ser� esticado na tela \n"
               "com o filtro especificado em 'Filtro \n"
               "Padr�o'. \n"
               " \n"
               "Se 'Tanto faz' estiver definido, a escala \n"
               "de 1x ou o esticamento para tela cheia ser�o \n"
               "usados dependendo se o primeiro foi ou n�o \n"
               "definido no �ltimo passo."
               );
         break;
      case MENU_LABEL_VIDEO_SHADER_NUM_PASSES:
         snprintf(s, len,
               "N�mero de Shaders. \n"
               " \n"
               "O RetroArch permite que voc� combine v�rios \n"
               "shaders com n�mero arbitr�rio de passos, filtros \n"
               "de hardware e fatores de escala personalizados. \n"
               " \n"
               "Essa op��o especifica o n�mero de passos a usar. \n"
               "Se for definido como 0 e usada a op��o Aplicar \n"
               "Altera��es de Shaders, ser� usado um shader vazio. \n"
               " \n"
               "A op��o Filtro Padr�o ir� afetar o filtro \n"
               "de esticamento.");
         break;
      case MENU_LABEL_VIDEO_SHADER_PARAMETERS:
         snprintf(s, len,
               "Par�metros de Shaders. \n"
               " \n"
               "Modifica o shader em uso diretamente. N�o ser� \n"
               "salvo no arquivo de predefini��es CGP/GLSLP.");
         break;
      case MENU_LABEL_VIDEO_SHADER_PRESET_PARAMETERS:
         snprintf(s, len,
               "Par�metros de Predefini��es de Shader. \n"
               " \n"
               "Modifica as predefini��es de shader em uso no menu."
               );
         break;
      case MENU_LABEL_VIDEO_SHADER_PASS:
         snprintf(s, len,
               "Caminho do shader. \n"
               " \n"
               "Todos os shaders devem ser do mesmo \n"
               "tipo (i.e. CG, GLSL ou HLSL). \n"
               " \n"
               "Defina o Diret�rio de Shaders para indicar \n"
               "onde o buscador come�a a procurar pelos \n"
               "shaders."
               );
         break;
      case MENU_LABEL_CONFIG_SAVE_ON_EXIT:
         snprintf(s, len,
               "Salva configura��o ao sair. �til para\n"
               "o menu, pois as defini��es podem ser\n"
               "modificadas. Sobrescreve a configura��o.\n"
               " \n"
               "#includes e coment�rios n�o s�o \n"
               "preservados. \n"
               " \n"
               "Por design, o arquivo de configura��o \n"
               "� considerado imut�vel, pois ele � \n"
               "provavelmente mantido pelo usu�rio, \n"
               "e n�o deve ser sobrescrito sem o \n"
               "seu conhecimento."
#if defined(RARCH_CONSOLE) || defined(RARCH_MOBILE)
               "\nPor�m, isso n�o funciona assim nos \n"
               "consoles, pois abrir o arquivo de \n"
               "configura��o manualmente n�o � \n"
               "realmente uma op��o."
#endif
               );
         break;
      case MENU_LABEL_VIDEO_SHADER_FILTER_PASS:
         snprintf(s, len,
               "Filtro de hardware para este passo. \n"
               " \n"
               "Se 'Tanto faz' estiver definido, o 'Filtro \n"
               "Padr�o' ser� usado."
               );
         break;
      case MENU_LABEL_AUTOSAVE_INTERVAL:
         snprintf(s, len,
               "Salva automaticamente a SRAM \n"
               "em intervalos regulares.\n"
               " \n"
               "Est� desativado por padr�o. O intervalo � \n"
               "medido em segundos. \n"
               " \n"
               "Um valor de 0 desativa o salvamento \n"
               "autom�tico.");
         break;
      case MENU_LABEL_INPUT_BIND_DEVICE_TYPE:
         snprintf(s, len,
               "Tipo de Dispositivo de Entrada. \n"
               " \n"
               "Escolhe o dispositivo a usar. Isso � \n"
               "relevante para o core libretro."
               );
         break;
      case MENU_LABEL_LIBRETRO_LOG_LEVEL:
         snprintf(s, len,
               "Define o n�vel de registro para os cores \n"
               "(GET_LOG_INTERFACE). \n"
               " \n"
               " Se o n�vel de registro de um core \n"
               " libretro estiver abaixo de libretro_log, \n"
               " esse ser� ignorado.\n"
               " \n"
               " Registros DEBUG s�o sempre ignorados a menos \n"
               " que o modo prolixo seja ativado (--verbose).\n"
               " \n"
               " DEBUG = 0\n"
               " INFO  = 1\n"
               " WARN  = 2\n"
               " ERROR = 3"
               );
         break;
      case MENU_LABEL_STATE_SLOT_INCREASE:
      case MENU_LABEL_STATE_SLOT_DECREASE:
         snprintf(s, len,
               "Slot de Savestates.\n"
               " \n"
               " Com o slot definido em 0, o nome do Savestate \n"
               " ser� *.state (ou o que estiver definido em commandline).\n"
               "Se diferente de 0, o nome ser� (caminho)(d), \n"
               "em que (d) � o n�mero do slot.");
         break;
      case MENU_LABEL_SHADER_APPLY_CHANGES:
         snprintf(s, len,
               "Aplicar Altera��es de Shaders. \n"
               " \n"
               "Ap�s alterar defini��es de shaders, use-o para \n"
               "aplicar as mudan�as. \n"
               " \n"
               "Mudar defini��es de shaders � uma opera��o \n"
               "computacionalmente cara e deve ser \n"
               "realizada explicitamente. \n"
               " \n"
               "Quando se aplicam shaders, as defini��es do menu \n"
               "de shaders s�o salvas em um arquivo tempor�rio \n"
               "(menu.cgp ou menu.glslp) e carregadas. O arquivo \n"
               "persiste ao fechar o RetroArch. Ele � salvo \n"
               "no Diret�rio de Shaders."
               );
         break;
      case MENU_LABEL_INPUT_BIND_DEVICE_ID:
         snprintf(s, len,
               "Dispositivo de Entrada. \n"
               " \n"
               "Escolhe um gamepad para o usu�rio N. \n"
               "O nome do pad est� dispon�vel."
               );
         break;
      case MENU_LABEL_MENU_TOGGLE:
         snprintf(s, len,
               "Alterna menu.");
         break;
      case MENU_LABEL_GRAB_MOUSE_TOGGLE:
         snprintf(s, len,
               "Alterna uso de mouse.\n"
               " \n"
               "Quando o mouse � usado, RetroArch o esconde e \n"
               "mant�m o seu ponteiro dentro da janela para \n"
               "permitir que a entrada relativa do mouse \n"
               "funcione melhor.");
         break;
      case MENU_LABEL_DISK_NEXT:
         snprintf(s, len,
               "Circula por imagens de discos. Usar \n"
               "ap�s ejetar. \n"
               " \n"
               " Finaliza ao usar ejetar novamente.");
         break;
      case MENU_LABEL_VIDEO_FILTER:
#ifdef HAVE_FILTERS_BUILTIN
         snprintf(s, len,
               "Filtro de v�deo baseado em CPU.");
#else
         snprintf(s, len,
               "Filtro de v�deo baseado em CPU.\n"
               " \n"
               "Caminho para uma biblioteca din�mica.");
#endif
         break;
      case MENU_LABEL_AUDIO_DEVICE:
         snprintf(s, len,
               "Sobrep�e-se ao dispositivo de �udio padr�o \n"
               "que est� em uso.\n"
               "� dependente do driver. \n"
#ifdef HAVE_ALSA
               " \n"
               "ALSA precisa de um dispositivo PCM."
#endif
#ifdef HAVE_OSS
               " \n"
               "OSS precisa de um caminho (ex.: /dev/dsp)."
#endif
#ifdef HAVE_JACK
               " \n"
               "JACK precisa de portas (ex.: system:playback1\n"
               ",system:playback_2)."
#endif
#ifdef HAVE_RSOUND
               " \n"
               "RSound precisa de um endere�o IP para \n"
               "servidor RSound."
#endif
               );
         break;
      case MENU_LABEL_DISK_EJECT_TOGGLE:
         snprintf(s, len,
               "Alterna eje��o para discos.\n"
               " \n"
               "Usado para conte�dos multidiscos.");
         break;
      case MENU_LABEL_ENABLE_HOTKEY:
         snprintf(s, len,
               "Ativar outras hotkeys.\n"
               " \n"
               " Se esta hotkey � usada por teclado, joybutton \n"
               "ou joyaxis, todas as outras hotkeys ser�o \n"
               "desativadas a menos que esta hotkey esteja sendo \n"
               "usada ao mesmo tempo. \n"
               " \n"
               "Isso � �til para implementa��es RETRO_KEYBOARD que \n"
               "consultam uma grande �rea do teclado, cujo caminho \n"
               "deve estar livre das hotkeys.");
         break;
      case MENU_LABEL_REWIND_ENABLE:
         snprintf(s, len,
               "Ativa retrocesso.\n"
               " \n"
               "Essa op��o causa uma perda de desempenho, \n"
               "por isso est� desativada por padr�o.");
         break;
      case MENU_LABEL_LIBRETRO_DIR_PATH:
         snprintf(s, len,
               "Diret�rios de Cores. \n"
               " \n"
               "Um diret�rio onde s�o buscadas as \n"
               "implementa��es de cores libretro.");
         break;
      case MENU_LABEL_VIDEO_REFRESH_RATE_AUTO:
         {
            /* Work around C89 limitations */
            char u[501];
            char t[501];
            snprintf(u, sizeof(u),
                  "Taxa de Atualiza��o Autom�tica.\n"
                  " \n"
                  "A taxa de atualiza��o exata de nosso monitor (Hz).\n"
                  "� usada para calcular a taxa de entrada de �udio \n"
                  "com a f�rmula: \n"
                  " \n"
                  "audio_input_rate = game input rate * display \n"
                  "refresh rate / game refresh rate\n"
                  " \n");
            snprintf(t, sizeof(t),
                  "Se a implementa��o n�o informar valores, \n"
                  "valores NTSC ser�o assumidos por quest�o de \n"
                  "compatibilidade.\n"
                  " \n"
                  "Esse valor deve ficar pr�ximo de 60Hz para \n"
                  "evitar grande mudan�as de pitch. Se o monitor \n"
                  "n�o rodar a 60Hz, ou algo pr�ximo a isso, desative\n"
                  "o VSync, e deixe-o com valores padr�o.");
            strlcat(s, u, len);
            strlcat(s, t, len);
         }
         break;
      case MENU_LABEL_VIDEO_ROTATION:
         snprintf(s, len,
               "For�a uma certa rota��o da tela. \n"
               " \n"
               "A rota��o � adicionada a outras definidas\n"
               "por conjuntos de cores (veja Permitir\n"
               "Rota��o de V�deo).");
         break;
      case MENU_LABEL_VIDEO_SCALE:
         snprintf(s, len,
               "Resolu��o de tela cheia.\n"
               " \n"
               "Resolu��o 0 usa a resolu��o \n"
               "do ambiente.\n");
         break;
      case MENU_LABEL_FASTFORWARD_RATIO:
         snprintf(s, len,
               "Taxa de Avan�o R�pido."
               " \n"
               "A taxa m�xima na qual o conte�do ser�\n"
               "executado ao se usar o Avan�o R�pido.\n"
               " \n"
               " (Ex.: 5.0 para conte�do 60 fps => 300 fps \n"
               "m�ximo).\n"
               " \n"
               "RetroArch entra em modo sleep para assegurar \n"
               "que a taxa m�xima n�o ser� excedida.\n"
               "N�o confie que esse teto tenha exatid�o \n"
               "perfeita.");
         break;
      case MENU_LABEL_VIDEO_MONITOR_INDEX:
         snprintf(s, len,
               "Prefer�ncia de monitor.\n"
               " \n"
               "0 (padr�o) significa nenhum monitor � \n"
               "preferido, 1 e demais (1 � o primeiro \n"
               "monitor), sugere ao RetroArch usar esse \n"
               "monitor em particular.");
         break;
      case MENU_LABEL_VIDEO_CROP_OVERSCAN:
         snprintf(s, len,
               "For�a o descarte de quadros overscanned. \n"
               " \n"
               "O comportamento exato dessa op��o � \n"
               "espec�fico da implementa��o do core.");
         break;
      case MENU_LABEL_VIDEO_SCALE_INTEGER:
         snprintf(s, len,
               "S� interpola v�deo em escalas m�ltiplas \n"
               "inteiras da resolu��o nativa.\n"
               " \n"
               "O tamanho base depende da geometria e da \n"
               "rela��o de aspecto informadas pelo sistema.\n"
               " \n"
               "Se For�ar Aspecto n�o estiver definida, X/Y \n"
               "ser�o escalonados em inteiros independentemente.");
         break;
      case MENU_LABEL_AUDIO_VOLUME:
         snprintf(s, len,
               "Volume de �udio, em dB.\n"
               " \n"
               " 0 dB � o volume normal. Nenhum ganho aplicado.\n"
               "O ganho pode ser controlado em execu��o com \n"
               "Aumentar Volume / Baixar Volume.");
         break;
      case MENU_LABEL_AUDIO_RATE_CONTROL_DELTA:
         snprintf(s, len,
               "Controle de taxa de �udio.\n"
               " \n"
               "Definindo como 0 desativa o controle de taxa.\n"
               "Outros valores controlam a varia��o da taxa \n"
               "de �udio.\n"
               " \n"
               "Define quanto de taxa de entrada pode ser \n"
               "regulada dinamicamente.\n"
               " \n"
               " Taxa de entrada � definida como: \n"
               " input rate * (1.0 +/- (rate control delta))");
         break;
      case MENU_LABEL_AUDIO_MAX_TIMING_SKEW:
         snprintf(s, len,
               "Distor��o de �udio m�xima.\n"
               " \n"
               "Define a m�xima varia��o da taxa de entrada.\n"
               "Voc� pode querer aument�-la para obter grandes\n"
               "varia��es no compasso, por exemplo, ao rodar\n"
               "cores PAL em telas NTSC, ao custo de um pitch\n"
               "de �udio inexato.\n"
               " \n"
               " A taxa de entrada � definida como: \n"
               " input rate * (1.0 +/- (max timing skew))");
         break;
      case MENU_LABEL_OVERLAY_NEXT:
         snprintf(s, len,
               "Alterna para o pr�ximo overlay.\n"
               " \n"
               "Navega��o circular.");
         break;
      case MENU_LABEL_LOG_VERBOSITY:
         snprintf(s, len,
               "Ativa ou desativa n�vel de prolixidade \n"
               "do frontend.");
         break;
      case MENU_LABEL_VOLUME_UP:
         snprintf(s, len,
               "Aumenta o volume de �udio.");
         break;
      case MENU_LABEL_VOLUME_DOWN:
         snprintf(s, len,
               "Baixa o volume de �udio.");
         break;
      case MENU_LABEL_VIDEO_DISABLE_COMPOSITION:
         snprintf(s, len,
               "Desativa composition � for�a.\n"
               "V�lido somente para Windows Vista/7 atualmente.");
         break;
      case MENU_LABEL_PERFCNT_ENABLE:
         snprintf(s, len,
               "Ativa ou desativa contadores de desempenho \n"
               "do frontend.");
         break;
      case MENU_LABEL_SYSTEM_DIRECTORY:
         snprintf(s, len,
               "Diret�rio system. \n"
               " \n"
               "Define o diret�rio 'system'.\n"
               "Cores podem consultar esse diret�rio\n"
               "para carregar BIOS, configura��es\n"
               "espec�ficas de sistemas, etc.");
         break;
      case MENU_LABEL_SAVESTATE_AUTO_SAVE:
         snprintf(s, len,
               "Automaticamente salva um Savestate ao fechar \n"
               "o RetroArch.\n"
               " \n"
               "RetroArch carregar� automaticamente qualquer\n"
               "Savestate com esse caminho ao iniciar se 'Carregar\n"
               "Savestate Automaticamente' estiver ativado.");
         break;
      case MENU_LABEL_VIDEO_THREADED:
         snprintf(s, len,
               "Usa driver de v�deo em thread.\n"
               " \n"
               "Usando isso pode melhorar o desempenho ao \n"
               "poss�vel custo de lat�ncia e mais engasgos \n"
               "de v�deo.");
         break;
      case MENU_LABEL_VIDEO_VSYNC:
         snprintf(s, len,
               "Sincronismo Vertical de v�deo.\n");
         break;
      case MENU_LABEL_VIDEO_HARD_SYNC:
         snprintf(s, len,
               "Tenta sincronizar CPU com GPU via \n"
               "hardware.\n"
               " \n"
               "Pode reduzir a lat�ncia ao custo de \n"
               "desempenho.");
         break;
      case MENU_LABEL_REWIND_GRANULARITY:
         snprintf(s, len,
               "Granularidade do retrocesso.\n"
               " \n"
               " Ao retroceder um n�mero definido de \n"
               "quadros, voc� pode retroceder v�rios \n"
               "quadros por vez, aumentando a velocidade \n"
               "de retrocesso.");
         break;
      case MENU_LABEL_SCREENSHOT:
         snprintf(s, len,
               "Tira uma foto da tela.");
         break;
      case MENU_LABEL_VIDEO_FRAME_DELAY:
         snprintf(s, len,
               "Define quantos milissegundos retardar \n"
               "ap�s o VSync antes de executar o core.\n"
               "\n"
               "Pode reduzir a lat�ncia ao custo de\n"
               "um maior risco de engasgo de v�deo.\n"
               " \n"
               "O valor m�ximo � 15.");
         break;
      case MENU_LABEL_VIDEO_HARD_SYNC_FRAMES:
         snprintf(s, len,
               "Define quantos quadros a CPU pode rodar \n"
               "adiante da GPU com a op��o 'Sincronismo \n"
               "de GPU via Hardware' ativada.\n"
               " \n"
               "O valor m�ximo � 3.\n"
               " \n"
               " 0: Sincroniza com GPU de imediato.\n"
               " 1: Sincroniza com quadro anterior.\n"
               " 2: Etc ...");
         break;
      case MENU_LABEL_VIDEO_BLACK_FRAME_INSERTION:
         snprintf(s, len,
               "Insere um quadro preto entre quadros. \n"
               " \n"
               "�til para monitores de 120 Hz ao rodar \n"
               "material de 60 Hz com elimina��o do efeito \n"
               "'ghosting'.\n"
               " \n"
               "A taxa de atualiza��o de v�deo deve ainda \n"
               "ser configurada como se fosse um monitor de \n"
               "60 Hz (divida a taxa de atualiza��o por 2).");
         break;
      case MENU_LABEL_RGUI_SHOW_START_SCREEN:
         snprintf(s, len,
               "Mostra a tela inicial no menu.\n"
               "� definida automaticamente como falso quando\n"
               "vista pela primeira vez.\n"
               " \n"
               "� atualizada na configura��o apenas quando a\n"
               "op��o 'Salvar Configura��o ao Sair' est� ativada.\n");
         break;
      case MENU_LABEL_CORE_SPECIFIC_CONFIG:
         snprintf(s, len,
               "Carrega uma configura��o espec�fica baseada \n"
               "no core que est� sendo usado.\n");
         break;
      case MENU_LABEL_VIDEO_FULLSCREEN:
         snprintf(s, len, "Alterna tela cheia.");
         break;
      case MENU_LABEL_BLOCK_SRAM_OVERWRITE:
         snprintf(s, len,
               "Previne SRAM de ser sobrescrita ao \n"
               "carregar Savestates.\n"
               " \n"
               "Pode potencialmente levar a jogos bugados.");
         break;
      case MENU_LABEL_PAUSE_NONACTIVE:
         snprintf(s, len,
               "Pausa a jogatina quando o foco da janela \n"
               "� perdido.");
         break;
      case MENU_LABEL_VIDEO_GPU_SCREENSHOT:
         snprintf(s, len,
               "Captura material gr�fico de sa�da da \n"
               "GPU se estiver dispon�vel.");
         break;
      case MENU_LABEL_SCREENSHOT_DIRECTORY:
         snprintf(s, len,
               "Diret�rio de Capturas de Tela. \n"
               " \n"
               "Diret�rio para guardar as capturas de tela."
               );
         break;
      case MENU_LABEL_VIDEO_SWAP_INTERVAL:
         snprintf(s, len,
               "Intervalo de Troca de VSync.\n"
               " \n"
               "Usa um intervalo de troca personalizado. \n"
               "Use-e para reduzir � metade a taxa de \n"
               "atualiza��o do monitor.");
         break;
      case MENU_LABEL_SAVEFILE_DIRECTORY:
         snprintf(s, len,
               "Diret�rio de Saves. \n"
               " \n"
               "Salva todos os arquivos de save (*.srm) nesse \n"
               "diret�rio. Isso inclui arquivos relacionados \n"
               "como .bsv, .rt, .psrm, etc...\n"
               " \n"
               "Pode ser sobreposto por op��es expl�citas de\n"
               "linha de comando.");
         break;
      case MENU_LABEL_SAVESTATE_DIRECTORY:
         snprintf(s, len,
               "Diret�rio de Savestates. \n"
               " \n"
               "Salva todos os Savestates (*.state) nesse \n"
               "diret�rio.\n"
               " \n"
               "Pode ser sobreposto por op��es expl�citas de\n"
               "linha de comando.");
         break;
      case MENU_LABEL_ASSETS_DIRECTORY:
         snprintf(s, len,
               "Diret�rio de Recursos (Assets). \n"
               " \n"
               " Essa localiza��o � consultada quando se \n"
               "tenta buscar pelo menu recursos (assets) \n"
               "carreg�veis, etc.");
         break;
      case MENU_LABEL_DYNAMIC_WALLPAPERS_DIRECTORY:
         snprintf(s, len,
               "Diret�rio de Pap�is de Parede Din�micos. \n"
               " \n"
               " O lugar para armazenar pap�is de parede que \n"
               "ser�o carregados dinamicamente pelo menu \n"
               "dependendo do contexto.");
         break;
      case MENU_LABEL_SLOWMOTION_RATIO:
         snprintf(s, len,
               "Taxa de c�mera lenta."
               " \n"
               "Quando ativado, o conte�do rodar� em velocidade\n"
               "reduzida por um fator.");
         break;
      case MENU_LABEL_INPUT_AXIS_THRESHOLD:
         snprintf(s, len,
               "Define o limiar de eixo.\n"
               " \n"
               "O quanto deve ser torcido um eixo para\n"
               "resultar em um bot�o pressionado.\n"
               " Valores poss�veis s�o [0.0, 1.0].");
         break;
      case MENU_LABEL_INPUT_TURBO_PERIOD:
         snprintf(s, len, 
               "Per�odo de turbo.\n"
               " \n"
               "Descreve a velocidade na qual se alternam\n"
               "os bot�es com turbo ativado."
               );
         break;
      case MENU_LABEL_INPUT_AUTODETECT_ENABLE:
         snprintf(s, len,
               "Ativa autodetec��o de entrada.\n"
               " \n"
               "Tentar� autoconfigurar joypads \n"
               "em um estilo Plug-and-Play.");
         break;
      case MENU_LABEL_CAMERA_ALLOW:
         snprintf(s, len,
               "Autorizar ou desautorizar o acesso da c�mera \n"
               "pelos cores.");
         break;
      case MENU_LABEL_LOCATION_ALLOW:
         snprintf(s, len,
               "Autorizar ou desautorizar o acesso de \n"
               "servi�os de localiza��o pelos cores.");
         break;
      case MENU_LABEL_TURBO:
         snprintf(s, len,
               "Ativar turbo.\n"
               " \n"
               "Segurando o turbo enquanto se pressiona outro \n"
               "bot�o permitir� que o bot�o entre em modo \n"
               "turbo em que o seu estado ser� modulado com \n"
               "um sinal peri�dico. \n"
               " \n"
               "A modula��o p�ra quando o pr�prio bot�o \n"
               "(n�o � o bot�o de turbo) � solto.");
         break;
      case MENU_LABEL_OSK_ENABLE:
         snprintf(s, len,
               "Ativar/desativar teclado na tela.");
         break;
      case MENU_LABEL_AUDIO_MUTE:
         snprintf(s, len,
               "Ligar/desligar �udio.");
         break;
      case MENU_LABEL_REWIND:
         snprintf(s, len,
               "Segure o bot�o para retroceder.\n"
               " \n"
               "Retrocesso deve estar ativado.");
         break;
      case MENU_LABEL_EXIT_EMULATOR:
         snprintf(s, len,
               "Tecla para sair corretamente do RetroArch."
#if !defined(RARCH_MOBILE) && !defined(RARCH_CONSOLE)
               "\nFechando-o de outra forma mais agressiva \n"
               "(SIGKILL, etc) sair� sem salvar RAM, etc.\n"
               "Em sistemas baseados em Unix,\n"
               "SIGINT/SIGTERM permite um\n"
               "fechamento correto."
#endif
               );
         break;
      case MENU_LABEL_LOAD_STATE:
         snprintf(s, len,
               "Carrega Savestates.");
         break;
      case MENU_LABEL_SAVE_STATE:
         snprintf(s, len,
               "Salva Savestates.");
         break;
      case MENU_LABEL_NETPLAY_FLIP_PLAYERS:
         snprintf(s, len,
               "Netplay inverte usu�rios.");
         break;
      case MENU_LABEL_CHEAT_INDEX_PLUS:
         snprintf(s, len,
               "Incrementa o �ndice de cheats.\n");
         break;
      case MENU_LABEL_CHEAT_INDEX_MINUS:
         snprintf(s, len,
               "Decrementa o �ndice de cheats.\n");
         break;
      case MENU_LABEL_SHADER_PREV:
         snprintf(s, len,
               "Aplica o shader anterior no diret�rio.");
         break;
      case MENU_LABEL_SHADER_NEXT:
         snprintf(s, len,
               "Aplica o pr�ximo shader no diret�rio.");
         break;
      case MENU_LABEL_RESET:
         snprintf(s, len,
               "Reinicia o conte�do.\n");
         break;
      case MENU_LABEL_PAUSE_TOGGLE:
         snprintf(s, len,
               "Alterna estado de pausado e n�o pausado.");
         break;
      case MENU_LABEL_CHEAT_TOGGLE:
         snprintf(s, len,
               "Alterna �ndice de cheats.\n");
         break;
      case MENU_LABEL_HOLD_FAST_FORWARD:
         snprintf(s, len,
               "Segure para avan�o r�pido. Soltando o bot�o \n"
               "desativa o avan�o r�pido.");
         break;
      case MENU_LABEL_SLOWMOTION:
         snprintf(s, len,
               "Segure para c�mera lenta.");
         break;
      case MENU_LABEL_FRAME_ADVANCE:
         snprintf(s, len,
               "O quadro avan�a quando o conte�do est� pausado.");
         break;
      case MENU_LABEL_MOVIE_RECORD_TOGGLE:
         snprintf(s, len,
               "Alterna entre estar gravando ou n�o.");
         break;
      case MENU_LABEL_L_X_PLUS:
      case MENU_LABEL_L_X_MINUS:
      case MENU_LABEL_L_Y_PLUS:
      case MENU_LABEL_L_Y_MINUS:
      case MENU_LABEL_R_X_PLUS:
      case MENU_LABEL_R_X_MINUS:
      case MENU_LABEL_R_Y_PLUS:
      case MENU_LABEL_R_Y_MINUS:
         snprintf(s, len,
               "Eixo para o anal�gico (esquema DualShock).\n"
               " \n"
               "Associa normalmente, por�m, se um anal�gico real \n"
               "� associado, pode ser lido como um anal�gico\n"
               "verdadeiro. \n"
               " \n"
               "Eixo positivo X � para direita. \n"
               "Eixo positivo Y � para baixo.");
         break;
      default:
         if (s[0] == '\0')
            strlcpy(s, menu_hash_to_str(MENU_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
         return -1;
   }

   return 0;
}
