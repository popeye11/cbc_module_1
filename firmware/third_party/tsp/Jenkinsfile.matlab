pipeline {
    agent {
        label "windows && cmake && msvc"
    }

    options {
        timestamps()
        buildDiscarder(logRotator(
            artifactNumToKeepStr: '1',
            numToKeepStr: '10')
        )
    }

    stages {
        stage('Checkout') {
            steps {
                dir('source') {
                    checkout(scm)
                }
            }
        }
        stage('Build MATLAB Toolbox') {
            steps {
                cmakeBuild(
                    generator: 'Visual Studio 15 2017 Win64',
                    buildDir: 'build',
                    buildType: 'Release',
                    sourceDir: 'source/matlab',
                    installation: 'InSearchPath',
                    cmakeArgs: '-D CMAKE_INSTALL_PREFIX=${WORKSPACE}/install/matlab',
                    cleanBuild: false,
                    steps: [[
                        args: '--config Release --target install',
                        withCmake: true,
                    ]]
                )
            }
        }
    }

    post {
        always {
            dir('install') {
                archiveArtifacts('matlab/**/*')
            }
        }
        cleanup {
            cleanWs()
        }
    }
}
