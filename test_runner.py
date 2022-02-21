import os
import argparse

def start(lognum):
    os.system("rm log1")
    os.system("make clean")
    os.system("make")

    for i in range(lognum):
        print(i + 1)
        # os.system("./logappend  -T " + str(i + 1) + " -K mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm -A -G " + str(i + 1) + " log1")
        os.system("./logappend  -T " + str(i + 1) + " -K secret -A -G " + str(i + 1) + " log1")

    print("Unlocking...")
    os.system("./unlock -K secret -T -G Jill log1")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Description of your program')
    parser.add_argument('-l','--lognum', help='Number of logs to add', required=True)
    args = vars(parser.parse_args())
    lognum=int(args['lognum'])
    start(lognum)
