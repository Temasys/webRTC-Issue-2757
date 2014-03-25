{
	'variables': 
	{
		'conditions': 
		[
			[ 'OS in ["linux", "freebsd", "openbsd", "solaris", "chromeos"]', 
			{
				# Use the systemwide Qt libs by default
				'variables': 
				{
				  'qt_sdk%': '/usr',
				},
				'qt_sdk': '<(qt_sdk)',
				'qt_moc%': '/usr/bin/moc-qt4',#'/usr/bin/moc', 
				'qt_includes': 
				[

					# Systemwide Qt libs are not contained under a single tree,
					# so we're adding package-qualified paths as a fallback.
					'<(qt_sdk)/include/qt4',
					'<(qt_sdk)/include/qt4/QtCore',
					'<(qt_sdk)/include/qt4/QtGui',
					'<(qt_sdk)/include/qt4/QtNetwork',
					'<(qt_sdk)/include/qt4/QtWebKit',
					
					#'<(qt_sdk)/include/qt5',
					#'<(qt_sdk)/include/qt5/QtCore',
					#'<(qt_sdk)/include/qt5/QtGui',
					#'<(qt_sdk)/include/qt5/QtNetwork',
					#'<(qt_sdk)/include/qt5/QtWebKit',
				],
				'qt_libs': 
				[
					'-lQtCore',
					'-lQtGui',
					'-lQtNetwork',
					'-lQtWebKit',
				],
			}],

			[ 'OS == "mac"', 
			{
				# Use the systemwide Qt libs by default
				'variables': 
				{
				  'qt_sdk%': '/Library/Frameworks',
				},
				'qt_sdk': '<(qt_sdk)',
				'qt_moc%': 'moc',
				'qt_includes': 
				[
					'<(qt_sdk)/QtCore.framework/Headers/',
					'<(qt_sdk)/QtGui.framework/Headers/',
					'<(qt_sdk)/QtNetwork.framework/Headers/',
					'<(qt_sdk)/QtWebKit.framework/Headers/',
				],
				'qt_libs': 
				[
					'<(qt_sdk)/QtCore.framework/',
					'<(qt_sdk)/QtGui.framework/',
					'<(qt_sdk)/QtNetwork.framework/',
					'<(qt_sdk)/QtWebKit.framework/',
				],
			}],

			[ 'OS == "win"', 
			{
				'variables': 
				{
				  # This is the default location for the version of Qt current on 10/11/12
				  'qt_sdk%': 'C:/Qt/4.8.5/',
				},
				'qt_sdk': '<(qt_sdk)',
				'qt_moc%': '<(qt_sdk)/bin/moc',
				'qt_includes': 
				[
					'<(qt_sdk)/include',
					'<(qt_sdk)/include/QtCore',
					'<(qt_sdk)/include/QtGui',
					'<(qt_sdk)/include/QtNetwork',
					'<(qt_sdk)/include/QtWebKit',
				],
				'qt_libs': 
				[
					'<(qt_sdk)/lib/QtCore4.lib',
					'<(qt_sdk)/lib/QtGui4.lib',
					'<(qt_sdk)/lib/QtNetwork4.lib',
					'<(qt_sdk)/lib/QtWebKit4.lib',
				],
			}],
		],#end conditions
	'moc_src_dir': 'AppRTCClient/Source/',
	'moc_gen_dir': 'AppRTCClient/Source/moc',
	},#end variable
	
	'includes': 
	[
		'../talk/build/common.gypi',
	],
	'copies': 
    [{
            'destination': '<(PRODUCT_DIR)/',
            'files': 
            [
                    '<(moc_src_dir)/captured-320x240-2s-48.frames',
            ],
    },],

	'conditions': 
	[
	    ['OS=="linux" or OS=="win" or OS=="mac"', 
	    {
	    'targets': [
	    #### target demoBug
		{
		
		'target_name': 'demoBug',
		'type': 'executable',
		'include_dirs' : 
		[
			'<@(qt_includes)',
		],
		'defines':
		[
			'QT_NO_EMIT',
		],
	    'sources': 
	    [
			'<(moc_src_dir)/ActivityManager.cpp',
			'<(moc_src_dir)/Conductor.cpp',
			'<(moc_src_dir)/MediaConstraintsNative.cpp',
			'<(moc_src_dir)/main.cpp',
	
	    
		    # MOC files
			'<(moc_gen_dir)/moc_ActivityManager.cpp',
			'<(moc_gen_dir)/moc_Conductor.cpp',
	    ],
	    'dependencies': 
	    [
			'<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
			'<(DEPTH)/talk/libjingle.gyp:libjingle_peerconnection',
			'debugger_qt_mocs',
	    ],
	    'link_settings': 
	    {
            'libraries': 
            [
              '<@(qt_libs)',
            ],
        },
	    'conditions': 
	    [
		    ['OS=="win" or OS=="mac"', 
		    {
				'sources': 
				[
					'<(moc_src_dir)/ActivityManager.h',
					'<(moc_src_dir)/Conductor.h',
					'<(moc_src_dir)/MediaConstraintsNative.h',
				],					
				#'msvs_settings': 
				#{
				#	'VCLinkerTool': 
				#	{
				#		#'SubSystem': '2',  # Windows
				#	},
				#},
		    }],  # OS=="win"
		    ['OS=="linux"', 
		    {
				'cflags': 
				[
					'<!@(pkg-config --cflags glib-2.0 gobject-2.0 )',
				],
				'link_settings': 
				{
					'ldflags': 
					[
						'<!@(pkg-config --libs-only-L --libs-only-other glib-2.0 gobject-2.0 gthread-2.0)'
					],
					'libraries': 
					[
						'<!@(pkg-config --libs-only-l glib-2.0 gobject-2.0 gthread-2.0)',
						'<@(qt_libs)',
						'-lX11',
						'-lXcomposite',
						'-lXext',
						'-lXrender',
					],
				},
		    }],  # OS=="linux"
		  ],  # conditions
		},  # target demoBug
	  ],#end targets
	}],#end conditions 'OS=="linux" or OS=="win"' or OS=="mac"

   	[ 'OS != "nacl"', 
	{
		'targets': 
		[{
			'target_name': 'debugger_qt_mocs',
			'type': 'none',
			'sources': 
			[
				'<(moc_src_dir)/ActivityManager.h',
				'<(moc_src_dir)/Conductor.h',
			],
			'rules': 
			[{
				'rule_name': 'generate_moc',
				'extension': 'h',
				'outputs': [ '<(moc_gen_dir)/moc_<(RULE_INPUT_ROOT).cpp' ],
				'action': [ '<(qt_moc)', '<(RULE_INPUT_PATH)','-o', '<(moc_gen_dir)/moc_<(RULE_INPUT_ROOT).cpp' ],
				'message': 'Generating <(RULE_INPUT_ROOT).cpp.',
			},],
		}],
	}],
  	], #end conditions
}
