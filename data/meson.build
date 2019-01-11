checkme_files = ['ecore', 'ecore_imf', 'efreet', 'emotion', 'ethumb', 'ethumb_client', 'evas']

if get_option('x11') == true
  checkme_files += 'ecore_x'
endif

if get_option('eeze') == true
  checkme_files += 'eeze'
endif

if get_option('elua') == true
  checkme_files += 'elua'
endif

foreach checkme : checkme_files
  install_data(join_paths(checkme, 'checkme'),
    install_dir : join_paths(dir_data, checkme)
  )
endforeach

subdir('eo')
subdir('edje')
subdir('embryo')
subdir(join_paths('ethumb', 'frames'))
subdir('elementary')
