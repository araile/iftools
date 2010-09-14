#!/usr/bin/env python
import os
import re
import subprocess
import sys
from collections import defaultdict
from fnmatch import fnmatch


ZCODE_PATH = os.environ.get('ZCODE_PATH', '') or os.environ.get('INFOCOM_PATH', '')
VALID_EXT = re.compile(r'^\.z[1-8]$')

normalise = lambda s: ''.join(s.split()).lower()

def run_game(fn):
    full_path = os.path.join(ZCODE_PATH, fn)
    try:
        result = subprocess.call(['frotz', '-d', '-q', '-Z', '0', full_path])
    except KeyboardInterrupt:
        result = None
    return result

def find_games(simple=False):
    games = [] if simple else defaultdict(dict)
    # add a / to path so root will start blank
    path = os.path.realpath(ZCODE_PATH) + os.path.sep
    for root, _, files in os.walk(path):
        author = os.path.basename(root).strip() or ''
        if not author or author.lower() in ('unsorted', 'misc'):
            author = '~Others'
        for file in files:
            title, ext = os.path.splitext(file)
            if not VALID_EXT.match(ext):
                continue
            fn = os.path.join(root, file)
            if simple:
                games.append((title, fn))
            else:
                games[author][title] = fn
    return games

def game_prompt(games):
    games = sorted(games)
    print 'Games:'
    for i, game in enumerate(games):
        print '%6d. %s' % (i + 1, game[0])
    try:
        selection = int(raw_input('\nChoose a game:\n>'))
    except ValueError:
        return None
    if 1 <= selection <= len(games):
        return games[selection - 1]
    else:
        return None

def play_game(pattern):
    pattern = '*%s*' % normalise(pattern)
    games = find_games(simple=True)
    matches = [g for g in games if fnmatch(normalise(os.path.basename(g[1])), pattern)]
    if len(matches) == 1:
        return run_game(matches.pop()[1])
    elif matches:
        try:
            game = game_prompt(matches)
        except KeyboardInterrupt:
            return
        if not game:
            print 'Invalid selection.'
            return 1
        run_game(game[1])
    else:
        print 'No games found.'
        return 1

def list_games():
    games = find_games(simple=False)
    for author, games in sorted(games.items()):
        print author.lstrip('~')
        for game, filename in sorted(games.items()):
            print '    %s' % game
        if not author.startswith('~'):
            print ''

def main(args):
    if not ZCODE_PATH:
        sys.stderr.write('error: ZCODE_PATH environment variable must be set\n')
        return 1
    if args:
        return play_game(' '.join(args))
    else:
        return list_games()

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
