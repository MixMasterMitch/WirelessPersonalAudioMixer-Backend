{
	'targets': [
		{
			'target_name': 'WirelessPersonalAudioMixerBackend',
			'sources': [
				'src/native/WirelessPersonalAudioMixerBackend.cpp',
				'src/native/UdpSender.cpp',
				'src/native/UdpReceiver.cpp',
				'src/native/OpusAudioEncoder.cpp',
				'src/native/OpusAudioDecoder.cpp',
				'src/native/AudioInputAndTransmitterThread.cpp',
				'src/native/AudioOutputThread.cpp',
				'src/native/AudioReceiverThread.cpp',
                'src/native/UselessTask.cpp'
			],
			'include_dirs': [
				'<(module_root_dir)/src/native/',
				'<(module_root_dir)/lib/portaudio/',
				'<(module_root_dir)/lib/opus/',
				"<!(node -e \"require('nan')\")"
			],
			"conditions" : [
				[
					'OS=="linux"', {
						"libraries" : [
							'<(module_root_dir)/lib/portaudio/libportaudio.a'
							'<(module_root_dir)/lib/opus/libopus.a',
						],
						'cflags!': [ '-fno-exceptions' ],
						'cflags_cc!': [ '-fno-exceptions' ],
						'cflags_cc': [ '-std=c++0x' ]
					}
				],
				[
					'OS=="mac"', {
						"libraries" : [
							'<(module_root_dir)/lib/portaudio/libportaudio.a',
							'<(module_root_dir)/lib/opus/libopus.a',
							'/Library/Frameworks/CoreAudio.framework',
							'/Library/Frameworks/AudioToolbox.framework',
							'/Library/Frameworks/AudioUnit.framework',
							'/Library/Frameworks/CoreServices.framework',
							'/Library/Frameworks/Carbon.framework'
						],
						'cflags!': [ '-fno-exceptions' ],
						'cflags_cc!': [ '-fno-exceptions' ],
						'cflags_cc': [ '-std=c++0x' ]
					}
				]
			]
		}
	]
}
