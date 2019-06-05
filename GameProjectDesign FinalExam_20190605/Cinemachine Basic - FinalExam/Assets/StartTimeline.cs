using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;

public class StartTimeline : MonoBehaviour
{
    public PlayableDirector _director;
    public Transform _playerCam;
    public Transform _timelineCam;

    private void OnTriggerEnter(Collider other)
    {
        Debug.Log("OnTriggerEnter named " + other.gameObject.name);
        if (_director != null && _director.state != PlayState.Playing)
        {
            _director.Play();
            _playerCam.gameObject.SetActive(false);
            _timelineCam.gameObject.SetActive(true);
        }
    }
    void OnEnable()
    {
        _director.stopped += OnPlayableDirectorStopped;
    }

    void OnPlayableDirectorStopped(PlayableDirector aDirector)
    {
        if (_director == aDirector)
        {
            _playerCam.gameObject.SetActive(true);
            _timelineCam.gameObject.SetActive(false);
        }
    }

    void OnDisable()
    {
        _director.stopped -= OnPlayableDirectorStopped;
    }
}
