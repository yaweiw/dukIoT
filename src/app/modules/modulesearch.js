Duktape.modSearch = function (id) {
    /* readFile() reads a file from disk, and returns a string or undefined.
     * 'id' is in resolved canonical form so it only contains terms and
     * slashes, and no '.' or '..' terms.
     */
    print('loading module:', id);
    var res;

    res = readFile('./' + id + '.js'); // so require('test2') not require('./test2')

    if (typeof res === 'string') {
        return res;
    }

    throw new Error('module not found: ' + id);
};