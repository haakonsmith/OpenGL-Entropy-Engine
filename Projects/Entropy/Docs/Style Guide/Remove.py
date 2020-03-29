import re

redundancy = [
    r".*// ?LOG(.+).*"
]


def clearRedundantComments(textList):
    cleanTextList = list()

    for line in textList:
        if not testRedundantLine(line):
            cleanTextList.append(line)
        else:
            print("Clearing: \n %s" % line)

    return cleanTextList


def testRedundantLine(line):
    for regex in redundancy:
        if re.match(regex, line):
            return True
    return False


