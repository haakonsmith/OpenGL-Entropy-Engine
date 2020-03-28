
copyright = """2020   Haakon Smith

               {} """


# def CopyRight(parameter_list):
withComment = "/*\n"
for line in copyright.splitlines():
    withComment+=(" * %s \n" % line)
withComment+=(" */\n")

print(withComment)
