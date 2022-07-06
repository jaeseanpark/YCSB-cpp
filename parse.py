from tqdm import tqdm
import sys

def check_duplicate(filename, recordcount):
    keys = set()
    with open(filename, 'r')as f:
        for line in f:
            keys.add(line)
    if len(keys) == recordcount:
        return True


def check_read():
    keys = dict()
    with open('writelog.txt', 'r') as fw:
        for line in tqdm(fw):
            keys[line] = 1
    with open('readlog.txt', 'r') as fu:
        for line in tqdm(fu):
            if line in keys.keys():
                keys[line] += 1
            else:
                print('key not recognizable')
    with open('read_compare.csv', 'w') as f:
        read_sorted = {k: v for k, v in sorted(keys.items(), key=lambda item: item[1], reverse=True)}
        for key, value in read_sorted.items():
            f.write(key[:-1] + ', ' + str(value) + '\n')


def check_keys():
    keys = dict()
    with open('writelog.txt', 'r') as fw:
        for line in tqdm(fw):
            keys[line] = 1
    with open('updatelog.txt', 'r') as fu:
        for line in tqdm(fu):
            if line in keys.keys():
                keys[line] += 1
            else:
                sys.exit('ERROR: key unrecognizable!!!!')
    with open('key_compare.csv', 'w') as f:
        keys_sorted = {k: v for k, v in sorted(keys.items(), key=lambda item: item[1], reverse=True)}
        for key, value in keys_sorted.items():
            f.write(key[:-1] + ', ' + str(value) + '\n')


def sort_file(filename):
    temp = []
    with open(filename, 'r') as f:
        for line in tqdm(f):
            temp.append(line)
        temp.sort()
    return temp


def main():
    # print(check_duplicate('writelog.txt', 1000000))
    check_keys()
    # print(check_duplicate('test.txt', 1000000))
    # check_read()
    # print('done')

    
    # read_sorted = sort_file('readlog.txt')
    # with open('readsorted.txt', 'w')as f:
    #     for line in read_sorted:
    #         f.write(line)

    # write_sorted = sort_file('writelog.txt')
    # with open('writesorted.txt', 'w')as f:
    #     for line in write_sorted:
    #         f.write(line)

    # keys = dict()
    # with open('writelog.txt', 'r') as fw:
    #     for line in tqdm(fw):
    #         keys[line] = 1
    # with open('test.txt', 'r') as fu:
    #     for line in tqdm(fu):
    #         if line in keys.keys():
    #             keys[line] += 1
    #         else:
    #             print('key not recognizable')


if __name__ == "__main__":
    main()