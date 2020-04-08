

import os, shutil

PROFILE_SAVE_PATH = "Tracing/Saved"

if not os.path.exists(PROFILE_SAVE_PATH):
    os.makedirs(PROFILE_SAVE_PATH)

dire = os.listdir(PROFILE_SAVE_PATH)

# i = max(dire, key=lambda x: x[-5])[-5] if len(dire) != 0 else 0
i = len(dire)

shutil.make_archive('Tracing/Saved/Session_save_' + str(int(i)),
                    'zip',
                    'Tracing',
                    'main.json')
