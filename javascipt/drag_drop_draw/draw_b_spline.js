//var strip = 'http://localhost:8000/b-spline-interpolation-left.json';
//var anulus = 'http://localhost:8000/quarter-anulus-2d.json';

// function file_buffer_to_texture(buffer){
//     console.log('array to texture');
//     console.log(buffer);
//     let view = new DataView(buffer);
//     console.log('view= ',view);
//     // for (let b of buffer){
//     //     console.log('b = ', b);
//     // }
//     //
//     // var png_buff = new PNG(buffer);
//     // // png_buff.decode('image.png',function (pixels){
//     // //     console.log('deconding...');
//     // // });
//     // console.log('array to texture');
//
//
//     var texture =1;
//     return texture;
// }

function draw_splines(data) {
    //let json = data[0];

    // fetch(data[0]).then(
    //     response => response.json()).then(
    //     json => {

    console.log('input data:', data);
    let json = data[0];
    //let texture_url = '../three.js/examples/textures/UV_Grid_Sm.jpg';
    let texture_url = data[1];//'./dummy_succeed/sub/square_grid.png'

    console.log(json);
    console.log(texture_url);

    var camera, scene, renderer;
    var mesh;

    init();
    animate();

    function init() {

        scene = new THREE.Scene();
        scene.background = new THREE.Color(0xf0f0f0);

        scene.add(new THREE.AmbientLight(0x808080));


        var light = new THREE.DirectionalLight(0xffffff, 1);
        light.position.set(1, 1, 1);
        scene.add(light);


        renderer = new THREE.WebGLRenderer({
            antialias: true
        });
        renderer.setPixelRatio(window.devicePixelRatio);
        renderer.setSize(window.innerWidth, window.innerHeight);

        camera = new THREE.PerspectiveCamera(50, window.innerWidth / window.innerHeight, .01, 2);
        //camera.position.x = .5;
        camera.position.z = 1;

        //camera.position.y = 1;



        var container = document.getElementById('container');
        container.appendChild(renderer.domElement);

        controls = new THREE.OrbitControls(camera, renderer.domElement);
        controls.addEventListener('change', render); // remove when using animation loop
        //controls.autoRotate = true
        // enable animation loop when using damping or autorotation
        //controls.enableDamping = true;
        //controls.dampingFactor = 0.25;
        controls.enableZoom = true;

        //camera.position.set( 0, 0, 0 );
        controls.update();

        //var texture = new THREE.TextureLoader().load( 'http://localhost:8000/chipping-plot-left.png' );


        var n_basis = json.b_spline.control_points.x.length;
        var cp_input = json.b_spline.control_points;
        var n_knt1 = json.b_spline.knot_vector.u.length;
        var n_knt2 = json.b_spline.knot_vector.v.length;

        var deg1 = json.b_spline.degree.u;
        var deg2 = json.b_spline.degree.v;

        var n_basis1 = n_knt1 - deg1 - 1;
        var n_basis2 = n_knt2 - deg2 - 1;

        console.log(n_knt1, n_knt2, deg1, deg2, n_basis1, n_basis2);


        var control_points = [];

        for (i = 0; i < n_basis1; i++) {
            var row = [];
            for (j = 0; j < n_basis2; j++) {
                var cp_id = j * n_basis1 + i;
                var v = new THREE.Vector4(cp_input.x[cp_id], cp_input.y[cp_id], cp_input.z[cp_id], 1)
                row.push(v);
                console.log(i, j, cp_id, v, row)
            }
            control_points.push(row);
        }

        console.log(control_points)
        var nurbsSurface = new THREE.NURBSSurface(deg1, deg2,
            json.b_spline.knot_vector.u, json.b_spline.knot_vector.v, control_points);



        function getSurfacePoint(u, v) {
            return nurbsSurface.getPoint(u, v);
        }

        var geometry = new THREE.ParametricBufferGeometry(getSurfacePoint, 100, 100);

        //var geometry = new THREE.BoxBufferGeometry( 1, 1, 1 );
        //var material = new THREE.MeshBasicMaterial( { map: texture } );

        //var map = new THREE.TextureLoader().load( 'textures/UV_Grid_Sm.jpg' );
        //map.wrapS = map.wrapT = THREE.RepeatWrapping;
        //map.anisotropy = 16;
        //var material = new THREE.MeshLambertMaterial( { map: map, side: THREE.DoubleSide } );

        //mesh = new THREE.Mesh( geometry, material );
        //scene.add( mesh );

        //var map = new THREE.TextureLoader().load('../three.js/examples/textures/UV_Grid_Sm.jpg');
        var map = new THREE.TextureLoader().load(texture_url);
        console.log('calling buffer to texture');
        map.wrapS = THREE.RepeatWrapping;
        map.repeat.x = - 1;
        map.wrapT = THREE.RepeatWrapping;
        map.repeat.y = - 1;
        //var txt = file_buffer_to_texture(data[1]);

        // var map = new THREE.Texture();
        // map.image = data[1];
        // data[1].onload = function() {
        //     map.needsUpdate = true;
        // };




        map.wrapS = map.wrapT = THREE.RepeatWrapping;
        map.anisotropy = 16;

        //var geometry = new THREE.ParametricBufferGeometry( getSurfacePoint, 20, 20 );
        var material = new THREE.MeshLambertMaterial({
            map: map,
            side: THREE.DoubleSide
        });
        var nurbs_surface = new THREE.Mesh(geometry, material);

        scene.add(nurbs_surface);

        // var cube = new THREE.Mesh( new THREE.CubeGeometry( 1, 1, 1 ), new THREE.MeshNormalMaterial() );
        // scene.add(cube);


        scene.add(get_square_00());
        scene.add(get_square_01());
        scene.add(get_square_02());

        scene.rotation.x = 90 * Math.PI / 180
        scene.rotation.y = 180 * Math.PI / 180



        //document.body.appendChild( renderer.domElement );

        //

        window.addEventListener('resize', onWindowResize, false);

    }

    function onWindowResize() {

        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();

        renderer.setSize(window.innerWidth, window.innerHeight);

    }

    function get_square_00() {
        var square00 = new THREE.Geometry();

        square00.vertices.push(new THREE.Vector3(-.05, -.5, 0));
        square00.vertices.push(new THREE.Vector3(-.05, .5, 0));
        square00.vertices.push(new THREE.Vector3(.05, .5, 0));
        square00.vertices.push(new THREE.Vector3(.05, -.5, 0));

        square00.faces.push(new THREE.Face3(0, 1, 2));
        square00.faces.push(new THREE.Face3(0, 3, 2));

        return new THREE.Mesh(square00, new THREE.MeshBasicMaterial({
            color: '#5e5e5e',
            side: THREE.DoubleSide,
            transparent: true,
            opacity: 0.5
        }));
    }

    function get_square_01() {
        var square00 = new THREE.Geometry();

        square00.vertices.push(new THREE.Vector3(0, -.5, -.05));
        square00.vertices.push(new THREE.Vector3(0, .5, -.05));
        square00.vertices.push(new THREE.Vector3(0, .5, .05));
        square00.vertices.push(new THREE.Vector3(0, -.5, .05));

        square00.faces.push(new THREE.Face3(0, 1, 2));
        square00.faces.push(new THREE.Face3(0, 3, 2));

        return new THREE.Mesh(square00, new THREE.MeshBasicMaterial({
            color: '#5e5e5e',
            side: THREE.DoubleSide,
            transparent: true,
            opacity: 0.5
        }));
    }


    function get_square_02() {
        var square00 = new THREE.Geometry();

        square00.vertices.push(new THREE.Vector3(-.05, 0, -.05));
        square00.vertices.push(new THREE.Vector3(.05, 0, -.05));
        square00.vertices.push(new THREE.Vector3(.05, 0, .05));
        square00.vertices.push(new THREE.Vector3(-.05, 0, .05));

        square00.faces.push(new THREE.Face3(0, 1, 2));
        square00.faces.push(new THREE.Face3(0, 3, 2));

        return new THREE.Mesh(square00, new THREE.MeshBasicMaterial({
            color: '#5e5e5e',
            side: THREE.DoubleSide,
            transparent: true,
            opacity: 0.5
        }));
    }

    function animate() {

        requestAnimationFrame(animate);

        //mesh.rotation.x += 0.005;
        //mesh.rotation.y += 0.01;

        controls.update(); // required if controls.enableDamping = true, or if controls.autoRotate = true
        //stats.update();

        render();

    }


    function render() {
        renderer.render(scene, camera);
    }

    //});

}
