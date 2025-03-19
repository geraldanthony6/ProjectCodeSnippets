using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using TMPro;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;
using UnityEngine.Video;

public class FloatingGameManger : MonoBehaviour
{
    public static FloatingGameManger Instance {get; private set; }

    public static List<GameObject> SpawnedTargets { get; private set; }

    public delegate void GemManipulated();
    public static event GemManipulated SpawnedEvent;
    public static event GemManipulated DestroyedEvent;

    [SerializeField] private GameObject[] ObjectPrefabs;
    [SerializeField] private GameObject TutorialScreen;
    [SerializeField] private GameObject PlayerCanvas;
    [SerializeField] private GameObject LoreCanvas;
    [SerializeField] private GameObject InstructionsCanvas;
    [SerializeField] private GameObject GameOverCanvas;
    [SerializeField] private GameObject TableCanvas;
    [SerializeField] private GameObject PracticeRoomCanvas;
    [SerializeField] private GameTimer GameTimer;
    [SerializeField] private GameObject[] GemsCollected;
    [SerializeField] private Transform[] SpawnPoints;
    [SerializeField] private List<GameObject> CurrentOccupiedPoints;
    [SerializeField] private List<GameObject> CurrentSpawnedObjects;
    [SerializeField] private int ObjectsSpawned;
    
    [SerializeField] private TextMeshProUGUI RoundText;
    [SerializeField] private TextMeshProUGUI ScoreText;
    [SerializeField] private TextMeshProUGUI TutorialScoreText;
    [SerializeField] private TextMeshProUGUI GameOverScoreText;
    [SerializeField] private TextMeshProUGUI GameOverRoundText;
    
    [SerializeField] private Podium GameAreaPodium;
    
    [SerializeField] private AudioSource BackGroundMusic;
    [SerializeField] private AudioSource PlayerAudioSource;
    [SerializeField] private AudioClip UIClip;
    [SerializeField] private AudioClip BadUIInputClip;
    [SerializeField] private AudioClip RoundCompleteClip;
    [SerializeField] private AudioClip GameOverClip;
    
    [SerializeField] private InputActionReference StartInput;
    [SerializeField] private InputActionAsset ActionAsset;

    [SerializeField] private bool bIsStarted;
    [SerializeField] private bool bIsNeedleHandInCorrectPosition;
    [SerializeField] private bool bIsFlashlightHandInCorrectPosition;
    [SerializeField] private bool bIsPracticeRoom;
    [SerializeField] private bool bTutorialChosen;
    [SerializeField] private bool bPracticeChosen;
    [SerializeField] private bool bIsInPractice;
    [SerializeField] private int CollectedGemIndex;
    private bool StartInputPressed;
    private bool VideoCompleted;
    private int CurrentOccupiedIndex;
    [SerializeField] private VideoPlayer LoreVideo;
    [SerializeField] private int Score;
    [SerializeField] public float InteractBuffer;
    private int CurrentMaxScore;
    private bool bCanSpawn;
    private bool bGameOver;
    private bool bIsAtStartingPoint;
    private bool bIsPodiumInPosition;
    public bool bIsDamaged;
    private bool bGameOverSoundPlayed = false;
    private int TutorialCount;
    private float InputBuffer;
    private int CurrentRound;
    private int ObjectSpawnLengthAllowed;
    private int TutorialMax;
    private bool AdjustLight;
    private float LevelLightColor;
    [SerializeField] private Color StartingSkyColor;
    [SerializeField] private Color StartingEquatorColor;

    // new variable added
    public int adjustedCurrentRound = GameData.LevelSelected;

    private void Awake()
    {
        // Initialize the static instance
        if(Instance != null)
        {
            Debug.LogError("More than one game manager system! " + transform + " - " + Instance);
            Destroy(gameObject);
            return;
        }
        Instance = this;

        SetSoudsVolume();

    }

    // Start is called before the first frame update
    void Start()
    {
        // Initialize variables
        adjustedCurrentRound = GameData.LevelSelected;
        ObjectSpawnLengthAllowed = adjustedCurrentRound;
        
        CurrentMaxScore = 18;
        CurrentRound = adjustedCurrentRound;
        TutorialCount = 0;
        ObjectsSpawned = 1;
        bCanSpawn = false;
        AdjustLight = false;
        LevelLightColor = 1.0f;
        StartingSkyColor = RenderSettings.ambientSkyColor;
        StartingEquatorColor = RenderSettings.ambientEquatorColor;
        
        RoundText.text = CurrentRound.ToString();
    }

    // Update is called once per frame
    void Update()
    {
        if (!bIsPracticeRoom)
        { 
            // Spawn objects when the current set has been acquired
            if (ObjectsSpawned <= 0 && bCanSpawn && bIsStarted)
            {
                CurrentOccupiedPoints.Clear();
                try
                {
                    foreach (GameObject Gem in CurrentSpawnedObjects)
                    {
                        RemoveObjectFromList(Gem);
                        Destroy(Gem);
                    }
                    SpawnObjects(10);
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                    throw;
                }
            }
            
            // Handle input for moving through UI
            StartInputPressed = (StartInput.action.ReadValue<float>() > 0.5f) ? true : false;

            if (StartInputPressed && !bIsAtStartingPoint)
            {
                // TODO: Indicate the player must move to the starting location
                //PlayerAudioSource.PlayOneShot(BadUIInputClip);
            }

            if (!VideoCompleted)
            {
                
            }
            
            // Initial screen
            if (StartInputPressed && !bIsStarted && !bGameOver && bIsAtStartingPoint && TutorialCount == 0 && InputBuffer <= 0.0f )
            {
                PlayerAudioSource.clip = UIClip;
                PlayerAudioSource.Play();
                LoreCanvas.SetActive(false);
                TutorialScreen.SetActive(true);
                TableCanvas.SetActive(true);
                TutorialCount++;
                InputBuffer = 0.5f;
            }
            
            // Start tutorial after podium is in the correct position
            if (StartInputPressed && !bIsStarted && bIsPodiumInPosition && !bGameOver && bIsAtStartingPoint && TutorialCount == 1 && InputBuffer <= 0.0f && bIsNeedleHandInCorrectPosition && bIsFlashlightHandInCorrectPosition)
            {
                SpawnObjects(3);
                PlayerAudioSource.clip = UIClip;
                PlayerAudioSource.Play();
                TutorialMax = CurrentSpawnedObjects.Count;
                TutorialScreen.SetActive(false);
                InstructionsCanvas.SetActive(true);
                TutorialCount++;
                InputBuffer = 0.5f;
            }
            
            // Start the actual game after the tutorial is completed
            if (StartInputPressed && !bIsStarted && !bGameOver && bIsAtStartingPoint && TutorialCount == 2 && InputBuffer <= 0.0f && Score >= 0)
            {
                PlayerAudioSource.clip = UIClip;
                PlayerAudioSource.Play();
                InstructionsCanvas.SetActive(false);
                PlayerCanvas.SetActive(true);
                AdjustLight = true; 
                ObjectsSpawned = 0;
                Score = 0;
                ScoreText.text = Score.ToString();
                CollectedGemIndex = 0;
                foreach (GameObject CollectedGem in GemsCollected)
                {
                    CollectedGem.SetActive(false);
                }
                bIsStarted = true;
                bCanSpawn = true;
                InputBuffer = 0.5f;
            }
            
            // Restart the game
            if (StartInputPressed && !bIsStarted && bGameOver)
            {
                GameOverCanvas.SetActive(false);
                PlayerCanvas.SetActive(true);
                Score = 0;
                PlayerAudioSource.clip = UIClip;
                PlayerAudioSource.Play();
                bGameOverSoundPlayed = false;
                CurrentRound = 0;
                bGameOver = false;
                RoundText.text = CurrentRound.ToString();
                ScoreText.text = " ";
                bIsStarted = true;
                GameTimer.SetIsGameRunning(true);
                bCanSpawn = true;
            }

            if (!GameTimer.GetIsGameRunning() && !bIsStarted)
            {
                bCanSpawn = false;
            }
            
            // Handle game ending
            if (bGameOver)
            {
                // Show results and add offer a restart
                foreach (GameObject SpawnedObject in CurrentSpawnedObjects)
                {
                    Destroy(SpawnedObject);
                }

                CurrentSpawnedObjects.Clear();
                
                foreach (GameObject GemUI in GemsCollected)
                {
                    GemUI.SetActive(false);
                }

                if (!bGameOverSoundPlayed)
                {
                    PlayerAudioSource.PlayOneShot(GameOverClip);
                    bGameOverSoundPlayed = true;
                }
                
                PlayerCanvas.SetActive(false);
                GameOverCanvas.SetActive(true);
                GameOverScoreText.text = "Gems Collected: " + Score;
                GameOverRoundText.text = "Treasure Pieces Acquired: " + CurrentRound; // "+1" making the game start from the level 1 not zero
                ObjectsSpawned = 0;
                CollectedGemIndex = 0;
                ObjectSpawnLengthAllowed = adjustedCurrentRound;
            }

            if (InputBuffer > 0.0f)
            {
                InputBuffer -= Time.deltaTime;
            }

            if (InteractBuffer > 0.0f)
            {
                InteractBuffer -= Time.deltaTime;
            }

            if (InteractBuffer <= 0.0f)
            {
                bIsDamaged = false;
            }

            if (AdjustLight == true && LevelLightColor > 0.0f)
            {
                LevelLightColor -= Time.deltaTime;
                RenderSettings.ambientSkyColor = new Color(LevelLightColor, LevelLightColor, LevelLightColor);
                RenderSettings.ambientEquatorColor = new Color(LevelLightColor, LevelLightColor, LevelLightColor);
            }
        }
        else
        {
            /* Handle practice room logic
             * 1. Initially load into the practice room
             * 2. Show a tutorial option and a practice option
             * 3. Tutorial Sequence
             *      a. Load into tutorial
             *      b. Video explanation of the purpose of this game
             *      c. Explain how to hold controllers and the buttons you need to press with animation or video
             *      d. Explain what the gems are and how they are collected
             *      e. After they accomplish all of these tasks, allow them to enter the actual game or to continue practicing
             * 4. Practice Room
             *      a. Just an endless loop of gems randomly spawning on the table
            */
            if (bTutorialChosen)
            {
                // Handle tutorial stuff
            }

            if (bPracticeChosen)
            {
                // Handle practice stuff
                if (bIsPodiumInPosition && !bIsStarted && !bIsInPractice)
                {
                    DisableAllCanvas();
                    PracticeRoomCanvas.SetActive(true);
                    
                    ObjectsSpawned = 0;
                    Score = 0;
                    ScoreText.text = Score.ToString();
                    CollectedGemIndex = 0;
                    foreach (GameObject CollectedGem in GemsCollected)
                    {
                        CollectedGem.SetActive(false);
                    }
                }

                if (bIsStarted && !bIsInPractice)
                {
                    PlayerCanvas.SetActive(true);
                    bIsInPractice = true;
                    bCanSpawn = true;
                }
                
                if (ObjectsSpawned <= 0 && bCanSpawn)
                {
                    CurrentOccupiedPoints.Clear();
                    try
                    {
                        foreach (GameObject Gem in CurrentSpawnedObjects)
                        {
                            RemoveObjectFromList(Gem);
                            Destroy(Gem);
                        }
                        SpawnObjects(10);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e);
                        throw;
                    }
                }
            }
        }
    
    }
    
    // Handle the spawning of the gems
    public void SpawnObjects(int NumToSpawn)
    {
        // TODO: Fix the bug that prevents gems from spawning
        
        int RandSpawnPointNum;

        for (int i = 0; i < NumToSpawn; i++)
        {
            int RandObjectNum = UnityEngine.Random.Range(0, ObjectSpawnLengthAllowed);
            RandSpawnPointNum = UnityEngine.Random.Range(0, SpawnPoints.Length);

            if (!CurrentOccupiedPoints.Contains(SpawnPoints[RandSpawnPointNum].gameObject))
            {
                if (RandObjectNum != 1)
                {
                    GameObject NewObject = Instantiate(ObjectPrefabs[RandObjectNum], SpawnPoints[RandSpawnPointNum].position, Quaternion.identity);
                    NewObject.transform.SetParent(GameAreaPodium.transform);
                    CurrentSpawnedObjects.Add(NewObject);
                    if (SpawnedEvent != null)
                    {
                        SpawnedTargets = CurrentSpawnedObjects;
                        SpawnedEvent();
                    }
                    CurrentOccupiedPoints.Add(SpawnPoints[RandSpawnPointNum].gameObject);
                    ObjectsSpawned++;
                }
                else
                {
                    GameObject NewObject = Instantiate(ObjectPrefabs[RandObjectNum], SpawnPoints[RandSpawnPointNum].position, Quaternion.identity);
                    CurrentSpawnedObjects.Add(NewObject);
                    if (SpawnedEvent != null)
                    {
                        SpawnedTargets = CurrentSpawnedObjects;
                        SpawnedEvent();
                    }
                    CurrentOccupiedPoints.Add(SpawnPoints[RandSpawnPointNum].gameObject);
                }
            }
        }
        
        CurrentOccupiedPoints.Clear();
    }
    
    // Update number of Gems in the level to handle respawning new Gems
    public void ChangeObjectsSpawnedNumber(int ValueToAdd)
    {
        ObjectsSpawned += ValueToAdd;
    }

    // Add to the player score
    public void AddToScore(int NewValue)
    {
        if (Score >= CurrentMaxScore)
        {
            PlayerAudioSource.clip = RoundCompleteClip;
            PlayerAudioSource.Play();
            CurrentRound++;
            RoundText.text = CurrentRound.ToString();
            CollectedGemIndex = 0;
            CurrentMaxScore += 18;
            GameTimer.AddTime(45.0f);
            
            foreach (GameObject CollectedGem in GemsCollected)
            {
                CollectedGem.SetActive(false);
            }

            if (DestroyedEvent != null)
            {
                DestroyedEvent();
            }

            if (ObjectSpawnLengthAllowed < ObjectPrefabs.Length)
            {
                ObjectSpawnLengthAllowed++;
            }
        }
        else
        {
            Score += NewValue;
            GemsCollected[CollectedGemIndex].SetActive(true);
            ScoreText.text = Score.ToString();
            if (InstructionsCanvas.activeInHierarchy)
            {
                TutorialScoreText.text = Score + "/" + TutorialMax;
            }
            CollectedGemIndex++;
            if (DestroyedEvent != null)
            {
                DestroyedEvent();
            }
            GameTimer.AddTime(4.0f);// time increase on collected gem
        }
    }

    // Remove player score
    public void RemoveScore(int AmountToSubtract, bool bIsBadGem = false)
    {
        if (Score > 0)
        {
            Score -= AmountToSubtract;
            ScoreText.text = Score.ToString();
            CollectedGemIndex--;
            GemsCollected[CollectedGemIndex].SetActive(false);

            if (DestroyedEvent != null && bIsBadGem)
            {
                DestroyedEvent();
            }
            GameTimer.AddTime(-4.0f); // time decrease on mistakes
            
        }
    }

    public bool GetIsStarted()
    {
        return bIsStarted;
    }

    public void SetIsStarted(bool NewBool)
    {
        bIsStarted = NewBool;
    }

    public void SetIsEnded(bool NewBool)
    {
        bGameOver = NewBool;
    }

    public void SetIsInStartingPosition(bool NewBool)
    {
        bIsAtStartingPoint = NewBool;
    }

    public void SetNeedleInCorrectPosition(bool NewBool)
    {
        bIsNeedleHandInCorrectPosition = NewBool;
    }

    public void SetFlashlightInCorrectPosition(bool NewBool)
    {
        bIsFlashlightHandInCorrectPosition = NewBool;
    }

    public bool GetNeedleInCorrectPosition()
    {
        return bIsNeedleHandInCorrectPosition;
    }

    public bool GetFlashlightInCorrectPosition()
    {
        return bIsFlashlightHandInCorrectPosition;
    }

    public bool GetPodiumInPosition()
    {
        return bIsPodiumInPosition;
    }

    public bool GetIsPracticeRoom()
    {
        return bIsPracticeRoom;
    }

    public bool GetIsInPractice()
    {
        return bIsInPractice;
    }

    public void SetPodiumInPosition(bool NewBool)
    {
        bIsPodiumInPosition = NewBool;
    }

    public void RemoveObjectFromList(GameObject ObjectToRemove)
    {
        CurrentSpawnedObjects.Remove(ObjectToRemove);
    }

    private void SetSoudsVolume()
    {
        if(SettingButtons.GetisMuted())
        {
            PlayerAudioSource.volume = 0;
            BackGroundMusic.volume = 0;
        }
        else
        {
            PlayerAudioSource.volume = SettingButtons.GetSFXVolume();
            BackGroundMusic.volume = SettingButtons.GetMusicVolume();
        }
    }
    
    // Disable all canvas
    private void DisableAllCanvas()
    {
        TutorialScreen.SetActive(false);
        PlayerCanvas.SetActive(false);
        LoreCanvas.SetActive(false);
        InstructionsCanvas.SetActive(false);
        GameOverCanvas.SetActive(false);
        TableCanvas.SetActive(false);
        PracticeRoomCanvas.SetActive(false);
    }

}
