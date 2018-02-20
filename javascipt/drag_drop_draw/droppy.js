(window => {
    const DATA_FILE = 'b-spline-refinement-right.json';
    const IMG_FILE = 'chipping-plot-right.png';
    const DIR_NAME = 'wearAnalysis';


    const body = window.document.querySelector('body');

    let TEXTURE_URL;

    body.ondragover = function(event) {
        event.stopPropagation();
        event.preventDefault();
    };
    body.ondragenter = function(event) {
        event.stopPropagation();
        event.preventDefault();
    };

    body.ondrop = function(event) {
        event.preventDefault();
        console.log('dropped', event);
        const dt = event.dataTransfer;
        if (!dt) {
            console.error('No Dt');
            return;
        }

        let is_dir_resolve;
        let is_dir_reject;
        const isDir = new window.Promise((resolve, reject) => {
            is_dir_resolve = resolve;
            is_dir_reject = reject;
        });
        let has_json_resolve;
        let has_json_reject;
        let json_found = false;
        const hasJSON = new window.Promise((resolve, reject) => {
            has_json_resolve = resolve;
            has_json_reject = reject;
        });
        let has_texture_resolve;
        let has_texture_reject;
        let texture_found = false;
        const hasTexture = new window.Promise((resolve, reject) => {
            has_texture_resolve = resolve;
            has_texture_reject = reject;
        });

        let has_subdir_resolve;
        let has_subdir_reject;
        let subdir_found = false;
        const hasSubdir = new window.Promise((resolve, reject) => {
            has_subdir_resolve = resolve;
            has_subdir_reject = reject;
        });

        isDir
            .then(() => window.Promise.all([hasJSON, hasTexture, hasSubdir]))
            .then(a => {
                console.log('would call "DRAW" with')
                console.log(a);
                window.draw_splines(a);
            })
            .catch(err => {
                console.error(err);
                // window.draw_error (err.message);
            });

        function read_json(f) {
            console.log('got json');
            json_found = true;
            f.file(file => {
                    console.log('got json file');
                    const reader = new window.FileReader();
                    reader.onload = () => {
                        try {
                            console.log('json read', reader.result);
                            const data = JSON.parse(reader.result);
                            console.log('json parsed', data);
                            has_json_resolve(data);
                        } catch (err) {
                            has_json_reject(err);
                        }
                    };
                    reader.onerror = () => {
                        has_json_reject(reader.error);
                    };
                    reader.readAsText(file);
                },
                err => has_json_reject(err)
            );
        }

        function read_texture(f) {
            console.log('got texture');
            texture_found = true;
            console.log(f)
            // if (f.isFile){
            //     has_texture_resolve(f);
            // } else {
            //      has_texture_reject(new Error('texture file, in not a file.'));
            // }
            f.file(file => {
                TEXTURE_URL = window.URL.createObjectURL(file);
                has_texture_resolve(TEXTURE_URL);
                    //console.log('got texture file');
                    //const reader = new window.FileReader();
                    //reader.onload = () => {
                    //    console.log('texture read', reader.result);
                    //
                    //};
                    //reader.onerror = () => {
                    //    has_texture_reject(reader.error);
                    //};
                    //reader.readAsArrayBuffer(file);
                },
                err => has_texture_reject(err)
            );
        }

        function read_subfolder(entry) {

            console.log('subfolder = ', entry);
            subdir_found = true;

            const reader = entry.createReader();
            reader.readEntries(
                function(results) {
                    has_subdir_resolve(entry.fullPath);
                    //console.log('directory content',results);

                    for (let f of results) {
                        console.log('sub - entry = ', f)

                        if (f.isFile && f.name === DATA_FILE) {
                            read_json(f);
                        }

                        if (f.isFile && f.name === IMG_FILE) {
                            read_texture(f);
                        }
                    }
                    if (!json_found) {
                        return has_json_reject(new Error('json not found'));
                    }
                    if (!texture_found) {
                        return has_texture_reject(new Error('json not found'));
                    }
                },
                function(error) {
                    has_subdir_reject(error);
                }
            );
        }

        function read_directory(entry) {

            console.log('event = ', entry);
            if (entry.isDirectory) {
                const reader = entry.createReader();
                reader.readEntries(
                    function(results) {
                        is_dir_resolve();

                        for (let f of results) {
                            console.log('entry = ', f)
                            if (f.isDirectory && f.name == DIR_NAME) {
                                read_subfolder(f)
                            }
                            console.log('subdir_found = ', subdir_found)
                        }
                        if (!subdir_found) {
                            return has_subdir_reject(new Error('subfolder not found'));
                        }
                    },
                    function(error) {
                        is_dir_reject(error);
                    });
            } else {
                is_dir_reject(new Error('not a directory'));
            }
        }

        const entry = dt.items[0].webkitGetAsEntry();
        read_directory(entry);

    };
})(window);
