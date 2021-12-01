pipeline {
    agent {
        docker {
            image 'dockerhub.toptica.com/ubuntu-20.04-for-arm-embedded-builds:20210615T092212.ebf6431'
            label 'docker'
        }
    }

    options {
        timestamps()
        buildDiscarder(logRotator(
            artifactNumToKeepStr: '10',
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
        stage('Build') {
            parallel {
                stage('Build artifacts') {
                    steps {
                        cmakeBuild(
                            generator: 'Ninja',
                            buildDir: 'build',
                            buildType: 'RelWithDebInfo',
                            sourceDir: 'source',
                            cmakeArgs: '-DCMAKE_TOOLCHAIN_FILE=../source/gnu-arm-none-eabi.toolchain.cmake',
                            installation: 'InSearchPath',
                            cleanBuild: true,
                            steps: [[
                                args: 'firmware',
                            ]]
                        )
                    }
                }
                //stage('Build documentation') {
                //    steps {
                //        cmakeBuild(
                //            generator: 'Ninja',
                //            buildDir: 'documentation',
                //            buildType: 'RelWithDebInfo',
                //            sourceDir: 'source',
                //            installation: 'InSearchPath',
                //            cleanBuild: true,
                //            steps: [[
                //                args: 'html-docs',
                //            ]]
                //        )
                //
                //        publishHTML([
                //            reportDir: 'documentation/html',
                //            reportFiles: 'index.html',
                //            reportName: 'Documentation',
                //            reportTitles: 'Documentation',
                //            keepAll: false,
                //            alwaysLinkToLastBuild: false,
                //            allowMissing: false,
                //        ]
                //        )
                //    }
                //}
            }
        }
    }
    post {
        always {
            dir('build') {
                archiveArtifacts('firmware/*.elf')
            }
            recordIssues tools: [gcc(), clangTidy()]
        }
        cleanup {
            cleanWs()
        }
    }
}
