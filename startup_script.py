# here just in case - not being used. See platformio.ini
Import("env") # type: ignore
def after_upload(source, target, env):
    print("Delaying upload...")
    import time
    time.sleep(1.5)
    print("Done.")
env.AddPostAction("upload", after_upload) # type: ignore