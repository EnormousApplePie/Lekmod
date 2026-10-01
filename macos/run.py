#!/usr/bin/env python3
"""Launch the isolated game executable and retain its diagnostic output."""
from pathlib import Path
import os
import subprocess
import time
from prepare_app import DEST

HERE=Path(__file__).resolve().parent
existing=subprocess.run(['pgrep','-x','Civilization V'],capture_output=True,text=True)
if existing.returncode == 0:
    raise SystemExit('Civilization V is already running; close it before launching this test copy.')
if existing.returncode != 1:
    raise SystemExit('Cannot check running game processes: '+existing.stderr)
executable=DEST/'Contents/MacOS/Civilization V'
log=HERE/'build/runtime.log'
with log.open('wb') as output:
    environment=os.environ.copy()
    # Aspyr's localization loader fails with the inherited C.UTF-8 locale.
    for name in ('LANG', 'LC_ALL', 'LC_CTYPE'):
        if environment.get(name) == 'C.UTF-8':
            environment[name] = 'en_US.UTF-8'
    environment['SteamAppId']='8930'
    environment['SteamGameId']='8930'
    game=subprocess.Popen([str(executable)],cwd=executable.parent,env=environment,
                          stdout=output,stderr=subprocess.STDOUT,start_new_session=True)
    (HERE/'build/game.pid').write_text(str(game.pid)+'\n')
    time.sleep(10)
    print(f'PID {game.pid}; status {game.poll()}; output {log}')
print(log.read_text(errors='replace')[-6000:])
