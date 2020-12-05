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

#include "../msg_hash.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Winvalid-source-encoding"
#endif

 /* IMPORTANT:
  * For non-english characters to work without proper unicode support,
  * we need this file to be encoded in ISO 8859-1 (Latin1), not UTF-8.
  * If you save this file as UTF-8, you'll break non-english characters
  * (e.g. German "Umlauts" and Portugese diacritics).
 */
/* DO NOT REMOVE THIS. If it causes build failure, it's because you saved the file as UTF-8. Read the above comment. */
extern const char force_iso_8859_1[sizeof("�������")==7+1 ? 1 : -1];

const char *msg_hash_to_str_es(uint32_t hash)
{
   switch (hash)
   {
      case MSG_PROGRAM:
         return "RetroArch";
      case MSG_MOVIE_RECORD_STOPPED:
         return "Deteniendo grabaci�n de v�deo.";
      case MSG_MOVIE_PLAYBACK_ENDED:
         return "Reproducci�n detenida.";
      case MSG_AUTOSAVE_FAILED:
         return "No se ha podido iniciar el autoguardado.";
      case MSG_NETPLAY_FAILED_MOVIE_PLAYBACK_HAS_STARTED:
         return "Se ha iniciado una reproducci�n. No se puede ejecutar el juego en red.";
      case MSG_NETPLAY_FAILED:
         return "Error al iniciar el juego en red.";
      case MSG_LIBRETRO_ABI_BREAK:
         return "se ha compilado con una versi�n distinta a esta implementaci�n de libretro.";
      case MSG_REWIND_INIT_FAILED_NO_SAVESTATES:
         return "Esta implementaci�n no admite guardados r�pidos. No se puede utilizar el rebobinado.";
      case MSG_REWIND_INIT_FAILED_THREADED_AUDIO:
         return "La implementaci�n utiliza sonido multin�cleo. No se puede utilizar el rebobinado.";
      case MSG_REWIND_INIT_FAILED:
         return "Error al iniciar el b�fer de rebobinado. Rebobinado desactivado.";
      case MSG_REWIND_INIT:
         return "Iniciando b�fer de rebobinado, tama�o:";
      case MSG_CUSTOM_TIMING_GIVEN:
         return "Se ha indicado un ritmo personalizado";
      case MSG_VIEWPORT_SIZE_CALCULATION_FAILED:
         return "�Error al calcular el tama�o de ventana! Se utilizar�n datos en bruto. Probablemente esto no acabe bien...";
      case MSG_HW_RENDERED_MUST_USE_POSTSHADED_RECORDING:
         return "El n�cleo Libretro se renderiza por hardware. Es necesario utilizar la grabaci�n post-shaders.";
      case MSG_RECORDING_TO:
         return "Grabando a";
      case MSG_DETECTED_VIEWPORT_OF:
         return "Ventana detectada:";
      case MSG_TAKING_SCREENSHOT:
         return "Capturando pantalla.";
      case MSG_FAILED_TO_TAKE_SCREENSHOT:
         return "Error al capturar pantalla.";
      case MSG_FAILED_TO_START_RECORDING:
         return "Error al comenzar a grabar.";
      case MSG_RECORDING_TERMINATED_DUE_TO_RESIZE:
         return "Grabaci�n terminada por cambio de tama�o.";
      case MSG_USING_LIBRETRO_DUMMY_CORE_RECORDING_SKIPPED:
         return "Se est� utilizando el n�cleo dummy de libretro. Anulando grabaci�n.";
      case MSG_UNKNOWN:
         return "Desconocido";
      case MSG_LOADING_CONTENT_FILE:
         return "Cargando archivo de contenido";
      case MSG_RECEIVED:
         return "recibido";
      case MSG_UNRECOGNIZED_COMMAND:
         return "Comando no reconocido";
      case MSG_SENDING_COMMAND:
         return "Enviando comando";
      case MSG_GOT_INVALID_DISK_INDEX:
         return "Se ha obtenido un �ndice de disco no v�lido.";
      case MSG_FAILED_TO_REMOVE_DISK_FROM_TRAY:
         return "Error al extraer el disco de la bandeja.";
      case MSG_REMOVED_DISK_FROM_TRAY:
         return "Se ha retirado el disco de la bandeja.";
      case MSG_VIRTUAL_DISK_TRAY:
         return "bandeja de disco virtual.";
      case MSG_FAILED_TO:
         return "Error:";
      case MSG_TO:
         return "a";
      case MSG_SAVING_RAM_TYPE:
         return "Guardando tipo de RAM";
      case MSG_SAVING_STATE:
         return "Guardando r�pidamente";
      case MSG_LOADING_STATE:
         return "Cargando r�pidamente";
      case MSG_FAILED_TO_LOAD_MOVIE_FILE:
         return "Error al cargar el archivo de pel�cula";
      case MSG_FAILED_TO_LOAD_CONTENT:
         return "Error al cargar el contenido";
      case MSG_COULD_NOT_READ_CONTENT_FILE:
         return "No se ha podido leer el archivo de contenido";
      case MSG_GRAB_MOUSE_STATE:
         return "Capturar estado de rat�n";
      case MSG_PAUSED:
         return "En pausa.";
      case MSG_UNPAUSED:
         return "Sin pausa.";
      case MSG_FAILED_TO_LOAD_OVERLAY:
         return "Error al cargar sobreimposici�n.";
      case MSG_FAILED_TO_UNMUTE_AUDIO:
         return "Error al recuperar el sonido.";
      case MSG_AUDIO_MUTED:
         return "Sonido silenciado.";
      case MSG_AUDIO_UNMUTED:
         return "Sonido recuperado.";
      case MSG_RESET:
         return "Reiniciar";
      case MSG_FAILED_TO_LOAD_STATE:
         return "Error al cargar r�pidamente desde";
      case MSG_FAILED_TO_SAVE_STATE_TO:
         return "Error al guardar r�pidamente a";
      case MSG_FAILED_TO_SAVE_SRAM:
         return "Error al guardar la SRAM";
      case MSG_STATE_SIZE:
         return "Tama�o de guardado r�pido";
      case MSG_FOUND_SHADER:
         return "Shader encontrado";
      case MSG_SRAM_WILL_NOT_BE_SAVED:
         return "No se guardar� la SRAM.";
      case MSG_BLOCKING_SRAM_OVERWRITE:
         return "Bloqueando sobrescritura de SRAM";
      case MSG_CORE_DOES_NOT_SUPPORT_SAVESTATES:
         return "El n�cleo no es compatible con los guardados r�pidos.";
      case MSG_SAVED_STATE_TO_SLOT:
         return "Guardando r�pidamente a la ranura";
      case MSG_SAVED_SUCCESSFULLY_TO:
         return "Partida guardada en";
      case MSG_BYTES:
         return "bytes";
      case MSG_CONFIG_DIRECTORY_NOT_SET:
         return "No se ha asignado la carpeta de configuraci�n. No se puede guardar la nueva configuraci�n.";
      case MSG_SKIPPING_SRAM_LOAD:
         return "Omitiendo carga de SRAM.";
      case MSG_APPENDED_DISK:
         return "Disco incorporado";
      case MSG_STARTING_MOVIE_PLAYBACK:
         return "Iniciando reproducci�n.";
      case MSG_FAILED_TO_REMOVE_TEMPORARY_FILE:
         return "Error al borrar el archivo temporal";
      case MSG_REMOVING_TEMPORARY_CONTENT_FILE:
         return "Borrando archivo temporal de contenido";
      case MSG_LOADED_STATE_FROM_SLOT:
         return "Carga r�pida desde la ranura";
      case MSG_COULD_NOT_PROCESS_ZIP_FILE:
         return "No se ha podido procesar el archivo ZIP.";
      case MSG_SCANNING_OF_DIRECTORY_FINISHED:
         return "Escaneado de carpetas terminado";
      case MSG_SCANNING:
         return "Escaneando";
      case MSG_REDIRECTING_CHEATFILE_TO:
         return "Redirigiendo archivo de trucos a";
      case MSG_REDIRECTING_SAVEFILE_TO:
         return "Redirigiendo partida guardada a";
      case MSG_REDIRECTING_SAVESTATE_TO:
         return "Redirigiendo guardado r�pido a";
      case MSG_SHADER:
         return "Shader";
      case MSG_APPLYING_SHADER:
         return "Aplicando shader";
      case MSG_FAILED_TO_APPLY_SHADER:
         return "Error al aplicar shader.";
      case MSG_STARTING_MOVIE_RECORD_TO:
         return "Iniciando grabaci�n en";
      case MSG_FAILED_TO_START_MOVIE_RECORD:
         return "Error al iniciar la grabaci�n.";
      case MSG_STATE_SLOT:
         return "Ranura de guardado r�pido";
      case MSG_RESTARTING_RECORDING_DUE_TO_DRIVER_REINIT:
         return "Reiniciando grabaci�n debido al reinicio de un controlador.";
      case MSG_SLOW_MOTION:
         return "C�mara lenta.";
      case MSG_SLOW_MOTION_REWIND:
         return "Rebobinar c�mara lenta.";
      case MSG_REWINDING:
         return "Rebobinando.";
      case MSG_REWIND_REACHED_END:
         return "Se ha llegado al final del b�fer de rebobinado.";
      case MSG_CHEEVOS_HARDCORE_MODE_ENABLE:
         return "Modo Extremo Activado: guardar estado y reboninar se han desactivado.";
      case MSG_TASK_FAILED:
        return "Error";
      case MSG_DOWNLOADING:
        return "Descargando";
      case MSG_EXTRACTING:
        return "Extrayendo";
      default:
         break;
   }

   return "null";
}
