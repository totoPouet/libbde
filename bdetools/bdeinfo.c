/*
 * Shows information obtained from a BitLocker Drive Encryption (BDE) encrypted volume
 *
 * Copyright (C) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bdeoutput.h"
#include "bdetools_libbde.h"
#include "bdetools_libcerror.h"
#include "bdetools_libclocale.h"
#include "bdetools_libcnotify.h"
#include "bdetools_libcstring.h"
#include "bdetools_libcsystem.h"
#include "info_handle.h"

info_handle_t *bdeinfo_info_handle = NULL;
int bdeinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use bdeinfo to determine information about a Windows NT BitLocker\n"
	                 " Drive Encrypted (BDE) volume\n\n" );

	fprintf( stream, "Usage: bdeinfo [ -o offset ] [ -p password ] [ -r password ]\n"
	                 "               [ -s filename ] [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-o:     specify the volume offset\n" );
	fprintf( stream, "\t-p:     specify the password/passphrase\n" );
	fprintf( stream, "\t-r:     specify the recovery password\n" );
	fprintf( stream, "\t-s:     specify the file containing the startup key.\n"
	                 "\t        typically this file has the extension .BEK\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for bdeinfo
 */
void bdeinfo_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "bdeinfo_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	bdeinfo_abort = 1;

	if( bdeinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     bdeinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libcsystem_file_io_close(
	     0 ) != 0 )
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libbde_error_t *error                                      = NULL;
	libcstring_system_character_t *option_password             = NULL;
	libcstring_system_character_t *option_recovery_password    = NULL;
	libcstring_system_character_t *option_startup_key_filename = NULL;
	libcstring_system_character_t *option_volume_offset        = NULL;
	libcstring_system_character_t *source                      = NULL;
	size_t string_length                                       = 0;
	char *program                                              = "bdeinfo";
	libcstring_system_integer_t option                         = 0;
	int result                                                 = 0;
	int verbose                                                = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "bdetools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( libcsystem_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	bdeoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "ho:p:r:s:vV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (libcstring_system_integer_t) 'p':
				option_password = optarg;

				break;

			case (libcstring_system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

			case (libcstring_system_integer_t) 's':
				option_startup_key_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				bdeoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libbde_notify_set_stream(
	 stderr,
	 NULL );
	libbde_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &bdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( option_password != NULL )
	{
		if( info_handle_set_password(
		     bdeinfo_info_handle,
		     option_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set password.\n" );

			goto on_error;
		}
	}
	if( option_recovery_password != NULL )
	{
		if( info_handle_set_recovery_password(
		     bdeinfo_info_handle,
		     option_recovery_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set recovery password.\n" );

			goto on_error;
		}
	}
	if( option_startup_key_filename != NULL )
	{
		if( info_handle_read_startup_key(
		     bdeinfo_info_handle,
		     option_startup_key_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to read startup key.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		string_length = libcstring_system_string_length(
				 option_volume_offset );

		if( libcsystem_string_decimal_copy_to_64_bit(
		     option_volume_offset,
		     string_length + 1,
		     (uint64_t *) &( bdeinfo_info_handle->volume_offset ),
		     &error ) != 1 )
		{
			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			bdeinfo_info_handle->volume_offset = 0;

			fprintf(
			 stderr,
			 "Unsupported volume offset defaulting to: %" PRIu64 ".\n",
			 bdeinfo_info_handle->volume_offset );
		}
	}
	result = info_handle_open_input(
	          bdeinfo_info_handle,
	          source,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "Unable to unlock keys.\n" );

		goto on_error;
	}
	if( info_handle_volume_fprint(
	     bdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print volume information.\n" );

		goto on_error;
	}
	if( info_handle_close(
	     bdeinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &bdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( bdeinfo_info_handle != NULL )
	{
		info_handle_free(
		 &bdeinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

