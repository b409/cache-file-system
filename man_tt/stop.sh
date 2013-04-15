#!/bin/bash
ps aux | grep ttserver | grep -v 'grep' | awk -F ' ' '{print $2}' | xargs kill -TERM
